import fs from 'node:fs/promises';
import http from 'node:http';
import path from 'node:path';
import { spawn } from 'node:child_process';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const WEB_ROOT = path.join(ROOT, 'web');
const HOST = '127.0.0.1';
const MAX_SOURCE_BYTES = 128 * 1024;
const MAX_REQUEST_BYTES = 140 * 1024;
const MAX_DIAGNOSTIC_BYTES = 64 * 1024;

const STATIC_FILES = new Map([
  ['/', ['index.html', 'text/html; charset=utf-8']],
  ['/index.html', ['index.html', 'text/html; charset=utf-8']],
  ['/styles.css', ['styles.css', 'text/css; charset=utf-8']],
  ['/app.js', ['app.js', 'text/javascript; charset=utf-8']]
]);

function securityHeaders(contentType) {
  return {
    'Content-Type': contentType,
    'Content-Security-Policy': "default-src 'self'; script-src 'self'; style-src 'self'; img-src 'self' data:; connect-src 'self'; object-src 'none'; base-uri 'none'; frame-ancestors 'none'",
    'Referrer-Policy': 'no-referrer',
    'X-Content-Type-Options': 'nosniff',
    'X-Frame-Options': 'DENY'
  };
}

function sendJson(response, status, value) {
  response.writeHead(status, {
    ...securityHeaders('application/json; charset=utf-8'),
    'Cache-Control': 'no-store'
  });
  response.end(`${JSON.stringify(value)}\n`);
}

function requestError(statusCode, message) {
  const error = new Error(message);
  error.statusCode = statusCode;
  return error;
}

async function readJsonBody(request) {
  if (!(request.headers['content-type'] ?? '').startsWith('application/json')) {
    throw requestError(415, 'content_type_must_be_application_json');
  }
  const chunks = [];
  let size = 0;
  for await (const chunk of request) {
    size += chunk.length;
    if (size > MAX_REQUEST_BYTES) throw requestError(413, 'request_too_large');
    chunks.push(chunk);
  }
  try {
    return JSON.parse(Buffer.concat(chunks).toString('utf8'));
  } catch {
    throw requestError(400, 'invalid_json');
  }
}

export function compileSource(source, {
  compiler = process.env.CXX || 'g++',
  cwd = ROOT,
  timeoutMs = 20_000
} = {}) {
  return new Promise((resolve) => {
    const diagnostics = [];
    let diagnosticBytes = 0;
    let truncated = false;
    let timedOut = false;
    let settled = false;

    const child = spawn(
      compiler,
      ['-std=c++20', '-fsyntax-only', '-x', 'c++', '-'],
      { cwd, shell: false, windowsHide: true, stdio: ['pipe', 'pipe', 'pipe'] }
    );

    const capture = (chunk) => {
      const remaining = MAX_DIAGNOSTIC_BYTES - diagnosticBytes;
      if (remaining <= 0) {
        truncated = true;
        return;
      }
      const accepted = chunk.subarray(0, remaining);
      diagnostics.push(accepted);
      diagnosticBytes += accepted.length;
      if (accepted.length < chunk.length) truncated = true;
    };
    child.stdout.on('data', capture);
    child.stderr.on('data', capture);

    const finish = (result) => {
      if (settled) return;
      settled = true;
      clearTimeout(timer);
      let output = Buffer.concat(diagnostics).toString('utf8').trim();
      if (!output && result.diagnostics) output = result.diagnostics;
      if (truncated) output += `${output ? '\n' : ''}[diagnostics truncated]`;
      resolve({ ...result, diagnostics: output, truncated });
    };

    const timer = setTimeout(() => {
      timedOut = true;
      child.kill();
    }, timeoutMs);

    child.once('error', (error) => {
      finish({
        status: 'compiler_unavailable',
        success: false,
        timedOut: false,
        exitCode: null,
        diagnostics: `Could not start ${compiler}: ${error.message}`
      });
    });
    child.once('close', (exitCode) => {
      finish({
        status: timedOut ? 'timeout' : exitCode === 0 ? 'compiled' : 'compile_error',
        success: !timedOut && exitCode === 0,
        timedOut,
        exitCode
      });
    });
    child.stdin.on('error', () => {});
    child.stdin.end(source);
  });
}

async function findProblemFiles(directory, result = []) {
  const entries = await fs.readdir(directory, { withFileTypes: true });
  for (const entry of entries) {
    const entryPath = path.join(directory, entry.name);
    if (entry.isDirectory()) {
      await findProblemFiles(entryPath, result);
    } else if (entry.isFile() && entry.name === 'problem.json') {
      result.push(entryPath);
    }
  }
  return result;
}

function requireString(value, field, file) {
  if (typeof value !== 'string' || value.length === 0) {
    throw new Error(`${file}: ${field} must be a non-empty string`);
  }
  return value;
}

function requireNumber(value, field, file) {
  if (!Number.isInteger(value) || value < 0) {
    throw new Error(`${file}: ${field} must be a non-negative integer`);
  }
  return value;
}

export function domainFor(stage, track) {
  if (stage === 'M' || track === 'math') return 'mathematics';
  if (stage === 'G' || track === 'algorithm' || track === 'algorithms') return 'algorithms';
  if (stage === 'F' || stage === 'A' || track === 'foundation' || track === 'advanced') {
    return 'data-structures';
  }
  return 'other';
}

export async function loadCatalog(projectRoot) {
  const problemsRoot = path.join(projectRoot, 'problems');
  const files = await findProblemFiles(problemsRoot);
  const problems = await Promise.all(files.map(async (file) => {
    const metadata = JSON.parse(await fs.readFile(file, 'utf8'));
    const directory = path.dirname(file);
    const relative = path.relative(problemsRoot, directory).split(path.sep);
    const id = requireString(metadata.id, 'id', file);
    if (id !== path.basename(directory)) {
      throw new Error(`${file}: id must match its directory name`);
    }
    const stage = requireString(metadata.stage, 'stage', file);
    const track = relative[0] ?? '';
    return {
      id,
      title: requireString(metadata.title, 'title', file),
      stage,
      difficulty: requireString(metadata.difficulty, 'difficulty', file),
      domain: domainFor(stage, track),
      track,
      topic: relative[1] ?? '',
      judgeMode: requireString(metadata.judge_mode, 'judge_mode', file),
      timeLimitMs: requireNumber(metadata.time_limit_ms, 'time_limit_ms', file),
      memoryLimitMb: requireNumber(metadata.memory_limit_mb, 'memory_limit_mb', file),
      expectedTime: requireString(metadata.expected_time, 'expected_time', file),
      expectedSpace: requireString(metadata.expected_space, 'expected_space', file),
      forbiddenSymbols: Array.isArray(metadata.forbidden_symbols)
        ? metadata.forbidden_symbols.filter((symbol) => typeof symbol === 'string')
        : [],
      statementPath: path.join(directory, 'statement.md')
    };
  }));
  problems.sort((left, right) => {
    const leftOrder = left.stage === 'F' ? 0 : 1;
    const rightOrder = right.stage === 'F' ? 0 : 1;
    return leftOrder - rightOrder || left.id.localeCompare(right.id);
  });
  const ids = new Set();
  for (const problem of problems) {
    if (ids.has(problem.id)) throw new Error(`duplicate problem ID: ${problem.id}`);
    ids.add(problem.id);
  }
  return problems;
}

function epochDayToIso(value) {
  if (!Number.isInteger(value) || value < 0) return null;
  return new Date(value * 86_400_000).toISOString().slice(0, 10);
}

export async function loadProgress(projectRoot) {
  const file = path.join(projectRoot, '.judge', 'progress.json');
  let store;
  try {
    store = JSON.parse(await fs.readFile(file, 'utf8'));
  } catch (error) {
    if (error?.code === 'ENOENT') return new Map();
    throw error;
  }
  if (![1, 2].includes(store.schema_version) || !Array.isArray(store.records)) {
    throw new Error('unsupported or malformed progress store');
  }
  const today = Math.floor(Date.now() / 86_400_000);
  const records = new Map();
  for (const record of store.records) {
    const problemId = requireString(record.problem_id, 'problem_id', file);
    if (records.has(problemId)) throw new Error(`duplicate progress ID: ${problemId}`);
    const attempts = requireNumber(record.attempts, 'attempts', file);
    const acceptedAttempts = requireNumber(
      record.accepted_attempts, 'accepted_attempts', file
    );
    if (acceptedAttempts > attempts) {
      throw new Error(`${file}: accepted_attempts cannot exceed attempts`);
    }
    const bestTimeMs = record.best_time_ms === null
      ? null
      : requireNumber(record.best_time_ms, 'best_time_ms', file);
    const reviewStreak = store.schema_version >= 2
      ? requireNumber(record.review_streak, 'review_streak', file)
      : 0;
    const lastAttemptDay = store.schema_version >= 2 && record.last_attempt_day !== null
      ? requireNumber(record.last_attempt_day, 'last_attempt_day', file)
      : null;
    const nextReviewDay = store.schema_version >= 2 && record.next_review_day !== null
      ? requireNumber(record.next_review_day, 'next_review_day', file)
      : null;
    records.set(problemId, {
      attempts,
      acceptedAttempts,
      bestTimeMs,
      lastVerdict: typeof record.last_verdict === 'string' ? record.last_verdict : '',
      reviewStreak,
      lastAttemptDate: epochDayToIso(lastAttemptDay),
      nextReviewDate: epochDayToIso(nextReviewDay),
      due: nextReviewDay !== null && nextReviewDay <= today
    });
  }
  return records;
}

function summary(problem, progress) {
  const { statementPath: _statementPath, ...metadata } = problem;
  return { ...metadata, progress: progress ?? null };
}

export async function createWebServer({
  projectRoot = ROOT,
  webRoot = WEB_ROOT,
  compileHandler = null
} = {}) {
  const catalog = await loadCatalog(projectRoot);
  const byId = new Map(catalog.map((problem) => [problem.id, problem]));
  const compile = compileHandler ?? ((source) => compileSource(source, { cwd: projectRoot }));
  let compileBusy = false;

  return http.createServer(async (request, response) => {
    try {
      const requestUrl = new URL(request.url ?? '/', `http://${HOST}`);
      if (requestUrl.pathname === '/api/compile') {
        if (request.method !== 'POST') {
          sendJson(response, 405, { error: 'method_not_allowed' });
          return;
        }
        const body = await readJsonBody(request);
        if (typeof body.problemId !== 'string' || !byId.has(body.problemId)) {
          sendJson(response, 404, { error: 'problem_not_found' });
          return;
        }
        if (typeof body.source !== 'string' || body.source.length === 0) {
          sendJson(response, 400, { error: 'source_must_be_non_empty' });
          return;
        }
        if (Buffer.byteLength(body.source, 'utf8') > MAX_SOURCE_BYTES) {
          sendJson(response, 413, { error: 'source_too_large' });
          return;
        }
        if (compileBusy) {
          sendJson(response, 429, { error: 'compile_busy' });
          return;
        }
        compileBusy = true;
        try {
          const result = await compile(body.source, { problemId: body.problemId });
          sendJson(response, 200, { result });
        } finally {
          compileBusy = false;
        }
        return;
      }

      if (request.method !== 'GET') {
        sendJson(response, 405, { error: 'method_not_allowed' });
        return;
      }

      if (requestUrl.pathname === '/api/health') {
        sendJson(response, 200, { status: 'ok', problemCount: catalog.length });
        return;
      }

      if (requestUrl.pathname === '/api/problems') {
        const progress = await loadProgress(projectRoot);
        sendJson(response, 200, {
          problems: catalog.map((problem) => summary(problem, progress.get(problem.id)))
        });
        return;
      }

      if (requestUrl.pathname.startsWith('/api/problems/')) {
        const problemId = decodeURIComponent(requestUrl.pathname.slice('/api/problems/'.length));
        const problem = byId.get(problemId);
        if (!problem) {
          sendJson(response, 404, { error: 'problem_not_found' });
          return;
        }
        const [statement, progress] = await Promise.all([
          fs.readFile(problem.statementPath, 'utf8'),
          loadProgress(projectRoot)
        ]);
        sendJson(response, 200, {
          problem: { ...summary(problem, progress.get(problem.id)), statement }
        });
        return;
      }

      if (requestUrl.pathname === '/api/reviews') {
        const progress = await loadProgress(projectRoot);
        const problems = catalog
          .filter((problem) => progress.get(problem.id)?.due)
          .map((problem) => summary(problem, progress.get(problem.id)))
          .sort((left, right) =>
            left.progress.nextReviewDate.localeCompare(right.progress.nextReviewDate) ||
            left.id.localeCompare(right.id)
          );
        sendJson(response, 200, { problems });
        return;
      }

      const asset = STATIC_FILES.get(requestUrl.pathname);
      if (!asset) {
        sendJson(response, 404, { error: 'not_found' });
        return;
      }
      const [fileName, contentType] = asset;
      const content = await fs.readFile(path.join(webRoot, fileName));
      response.writeHead(200, {
        ...securityHeaders(contentType),
        'Cache-Control': 'no-cache'
      });
      response.end(content);
    } catch (error) {
      if (!error.statusCode) console.error(error);
      if (!response.headersSent) {
        sendJson(
          response,
          error.statusCode ?? 500,
          { error: error.statusCode ? error.message : 'internal_error' }
        );
      }
      else response.end();
    }
  });
}

function parsePort(arguments_) {
  const index = arguments_.indexOf('--port');
  if (index === -1) return 4173;
  const port = Number(arguments_[index + 1]);
  if (!Number.isInteger(port) || port < 1 || port > 65_535) {
    throw new Error('--port requires an integer from 1 to 65535');
  }
  return port;
}

if (path.resolve(process.argv[1] ?? '') === fileURLToPath(import.meta.url)) {
  const port = parsePort(process.argv.slice(2));
  const server = await createWebServer();
  server.once('error', (error) => {
    if (error.code === 'EADDRINUSE') {
      console.error(`Port ${port} is already in use.`);
      console.error(`StructLab may already be running at http://${HOST}:${port}`);
      console.error('Choose another port with: npm run web -- --port <port>');
    } else {
      console.error(`Could not start StructLab Web: ${error.message}`);
    }
    process.exitCode = 1;
  });
  server.listen(port, HOST, () => {
    console.log(`StructLab Web: http://${HOST}:${port}`);
  });
}
