import fs from 'node:fs';
import path from 'node:path';
import { spawnSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const JUDGE = path.join(
  ROOT,
  'build',
  'mingw-debug',
  process.platform === 'win32' ? 'algo.exe' : 'algo'
);

function printHelp() {
  console.log(`Data Structures Practice

Usage:
  npm run practice -- <command> [arguments]

Commands:
  build                         Configure and build the local Judge
  list                          List available problems
  show <problem-id>             Show problem metadata and statement path
  new <problem-id> [source.cpp] Create a starter solution
  test <problem-id> [source.cpp]
                                Run deterministic Judge tests
  stress <problem-id> [source.cpp] [--seed N] [--cases N]
                                Run seeded differential tests
  progress [problem-id]         Show saved local Judge progress
  review                        List problems due for review
  check [test-regex]            Build and run C++ integration tests
  verify                        Run C++ tests and documentation checks

Default solution path:
  solutions/<problem-id>.cpp
`);
}

function run(command, args, options = {}) {
  const result = spawnSync(command, args, {
    cwd: ROOT,
    stdio: options.stdio ?? 'inherit',
    env: process.env
  });
  if (result.error) {
    throw new Error(`failed to start ${command}: ${result.error.message}`);
  }
  return result.status ?? 1;
}

function runRequired(command, args) {
  const status = run(command, args);
  if (status !== 0) process.exit(status);
}

function buildJudge() {
  runRequired('cmake', ['--preset', 'dev']);
  runRequired('cmake', ['--build', '--preset', 'dev', '--parallel']);
}

function ensureJudge() {
  if (!fs.existsSync(JUDGE)) buildJudge();
}

function findProblemDirectory(problemId, directory = path.join(ROOT, 'problems')) {
  for (const entry of fs.readdirSync(directory, { withFileTypes: true })) {
    if (!entry.isDirectory()) continue;
    const fullPath = path.join(directory, entry.name);
    if (entry.name === problemId &&
        fs.existsSync(path.join(fullPath, 'problem.json'))) {
      return fullPath;
    }
    const nested = findProblemDirectory(problemId, fullPath);
    if (nested) return nested;
  }
  return null;
}

function defaultSolutionPath(problemId) {
  return path.join(ROOT, 'solutions', `${problemId}.cpp`);
}

function relativeDisplay(filePath) {
  return path.relative(ROOT, filePath) || '.';
}

function createSolution(problemId, requestedPath) {
  if (!findProblemDirectory(problemId)) {
    throw new Error(`unknown problem ID: ${problemId}`);
  }
  const outputPath = requestedPath
    ? path.resolve(ROOT, requestedPath)
    : defaultSolutionPath(problemId);
  if (fs.existsSync(outputPath)) {
    throw new Error(`solution already exists: ${relativeDisplay(outputPath)}`);
  }
  fs.mkdirSync(path.dirname(outputPath), { recursive: true });
  fs.writeFileSync(
    outputPath,
    `// ${problemId}\n#include <iostream>\n#include <string>\n\nint main() {\n    // Read the statement, then implement the required ADT here.\n    return 0;\n}\n`,
    'utf8'
  );
  console.log(`Created ${relativeDisplay(outputPath)}`);
}

function parseSolutionArguments(command, args) {
  const problemId = args[0];
  if (!problemId) {
    throw new Error(`${command} requires a problem ID`);
  }
  const hasExplicitSource = args[1] && !args[1].startsWith('--');
  const sourcePath = hasExplicitSource
    ? path.resolve(ROOT, args[1])
    : defaultSolutionPath(problemId);
  if (!fs.existsSync(sourcePath)) {
    throw new Error(
      `solution not found: ${relativeDisplay(sourcePath)}\n` +
      `Create it with: npm run new:solution -- ${problemId}`
    );
  }
  return {
    problemId,
    sourcePath,
    options: args.slice(hasExplicitSource ? 2 : 1)
  };
}

const [command = 'help', ...args] = process.argv.slice(2);

try {
  if (command === 'help' || command === '--help' || command === '-h') {
    printHelp();
  } else if (command === 'build') {
    if (args.length !== 0) throw new Error('build does not accept arguments');
    buildJudge();
  } else if (command === 'list') {
    if (args.length !== 0) throw new Error('list does not accept arguments');
    ensureJudge();
    process.exit(run(JUDGE, ['list']));
  } else if (command === 'show') {
    if (args.length !== 1) throw new Error('show requires one problem ID');
    ensureJudge();
    process.exit(run(JUDGE, ['show', args[0]]));
  } else if (command === 'new') {
    if (args.length < 1 || args.length > 2) {
      throw new Error('new requires a problem ID and optional source path');
    }
    createSolution(args[0], args[1]);
  } else if (command === 'progress') {
    if (args.length > 1) throw new Error('progress accepts at most one problem ID');
    ensureJudge();
    process.exit(run(JUDGE, ['progress', ...args]));
  } else if (command === 'review') {
    if (args.length !== 0) throw new Error('review does not accept arguments');
    ensureJudge();
    process.exit(run(JUDGE, ['review']));
  } else if (command === 'test' || command === 'stress') {
    ensureJudge();
    const parsed = parseSolutionArguments(command, args);
    process.exit(run(JUDGE, [
      command,
      parsed.problemId,
      parsed.sourcePath,
      ...parsed.options
    ]));
  } else if (command === 'check') {
    if (args.length > 1) throw new Error('check accepts at most one test regex');
    buildJudge();
    const testArgs = ['--preset', 'dev', '--output-on-failure'];
    if (args[0]) testArgs.push('-R', args[0]);
    process.exit(run('ctest', testArgs));
  } else if (command === 'verify') {
    if (args.length !== 0) throw new Error('verify does not accept arguments');
    buildJudge();
    runRequired('ctest', ['--preset', 'dev', '--output-on-failure']);
    process.exit(run(process.execPath, ['scripts/docs-ready.mjs']));
  } else {
    throw new Error(`unknown command: ${command}`);
  }
} catch (error) {
  console.error(`Error: ${error.message}`);
  console.error('Run "npm run practice -- help" for usage.');
  process.exit(2);
}
