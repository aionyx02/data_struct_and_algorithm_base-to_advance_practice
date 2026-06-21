import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import os from 'node:os';
import path from 'node:path';
import { after, before, test } from 'node:test';
import { fileURLToPath } from 'node:url';

import { createWebServer } from '../scripts/web-server.mjs';

const repositoryRoot = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
let fixtureRoot;
let server;
let baseUrl;

before(async () => {
  fixtureRoot = await fs.mkdtemp(path.join(os.tmpdir(), 'structlab-web-test-'));
  const problemRoot = path.join(
    fixtureRoot, 'problems', 'foundation', 'stack', 'F03-stack-array'
  );
  await fs.mkdir(problemRoot, { recursive: true });
  await fs.writeFile(path.join(problemRoot, 'problem.json'), JSON.stringify({
    id: 'F03-stack-array',
    title: 'Fixed Capacity Array Stack',
    stage: 'F',
    difficulty: 'D1',
    judge_mode: 'stdio',
    time_limit_ms: 300,
    memory_limit_mb: 64,
    expected_time: 'O(1) per operation',
    expected_space: 'O(capacity)',
    forbidden_symbols: ['std::stack'],
    random_tests: 100,
    operation_limit: 100,
    tests_directory: 'tests'
  }));
  await fs.writeFile(
    path.join(problemRoot, 'statement.md'),
    '# Fixed Capacity Array Stack\n\nImplement a stack.\n'
  );
  await fs.mkdir(path.join(fixtureRoot, '.judge'), { recursive: true });
  await fs.writeFile(path.join(fixtureRoot, '.judge', 'progress.json'), JSON.stringify({
    schema_version: 2,
    records: [{
      problem_id: 'F03-stack-array',
      attempts: 2,
      accepted_attempts: 1,
      best_time_ms: 14,
      last_verdict: 'AC',
      review_streak: 1,
      last_attempt_day: 0,
      next_review_day: 0
    }]
  }));

  server = await createWebServer({
    projectRoot: fixtureRoot,
    webRoot: path.join(repositoryRoot, 'web')
  });
  await new Promise((resolve, reject) => {
    server.once('error', reject);
    server.listen(0, '127.0.0.1', resolve);
  });
  baseUrl = `http://127.0.0.1:${server.address().port}`;
});

after(async () => {
  await new Promise((resolve) => server.close(resolve));
  await fs.rm(fixtureRoot, { recursive: true, force: true });
});

test('local Web adapter serves the catalog with progress', async () => {
  const response = await fetch(`${baseUrl}/api/problems`);
  assert.equal(response.status, 200);
  assert.match(response.headers.get('content-security-policy'), /default-src 'self'/);
  const payload = await response.json();
  assert.equal(payload.problems.length, 1);
  assert.equal(payload.problems[0].id, 'F03-stack-array');
  assert.equal(payload.problems[0].progress.acceptedAttempts, 1);
  assert.equal(payload.problems[0].progress.due, true);
});

test('local Web adapter serves statement details and due reviews', async () => {
  const detailResponse = await fetch(`${baseUrl}/api/problems/F03-stack-array`);
  assert.equal(detailResponse.status, 200);
  const detail = await detailResponse.json();
  assert.match(detail.problem.statement, /Implement a stack/);

  const reviewResponse = await fetch(`${baseUrl}/api/reviews`);
  assert.equal(reviewResponse.status, 200);
  const reviews = await reviewResponse.json();
  assert.deepEqual(reviews.problems.map((problem) => problem.id), ['F03-stack-array']);
});

test('local Web adapter exposes only allowed routes and methods', async () => {
  const indexResponse = await fetch(`${baseUrl}/`);
  assert.equal(indexResponse.status, 200);
  assert.match(await indexResponse.text(), /StructLab/);

  assert.equal((await fetch(`${baseUrl}/package.json`)).status, 404);
  assert.equal((await fetch(`${baseUrl}/api/problems`, { method: 'POST' })).status, 405);
  assert.equal((await fetch(`${baseUrl}/api/problems/unknown`)).status, 404);
});
