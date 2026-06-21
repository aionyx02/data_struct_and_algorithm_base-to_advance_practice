const state = {
  problems: [],
  selectedId: null,
  track: 'all',
  difficulty: 'all',
  query: ''
};

const elements = {
  list: document.querySelector('#problem-list'),
  total: document.querySelector('#problem-total'),
  search: document.querySelector('#problem-search'),
  breadcrumbs: document.querySelector('#breadcrumbs'),
  index: document.querySelector('#problem-index'),
  title: document.querySelector('#problem-title'),
  metadata: document.querySelector('#metadata-row'),
  statement: document.querySelector('#problem-statement'),
  editor: document.querySelector('#draft-editor'),
  verdict: document.querySelector('#verdict-pill'),
  attempts: document.querySelector('#attempt-count'),
  accepted: document.querySelector('#accepted-count'),
  bestTime: document.querySelector('#best-time'),
  reviewDate: document.querySelector('#review-date'),
  reviewRow: document.querySelector('.review-row'),
  command: document.querySelector('#judge-command'),
  toast: document.querySelector('#toast'),
  sidebarToggle: document.querySelector('#sidebar-toggle')
};

function topicLabel(topic) {
  return topic.split('-').map((part) => part.charAt(0).toUpperCase() + part.slice(1)).join(' ');
}

function statusFor(problem) {
  if (problem.progress?.due) return { symbol: '↻', className: 'due', label: '待複習' };
  if ((problem.progress?.acceptedAttempts ?? 0) > 0) return { symbol: '✓', className: 'solved', label: '已通過' };
  if ((problem.progress?.attempts ?? 0) > 0) return { symbol: '•', className: 'attempted', label: '已嘗試' };
  return { symbol: '○', className: '', label: '未作答' };
}

function filteredProblems() {
  const query = state.query.trim().toLocaleLowerCase();
  return state.problems.filter((problem) => {
    const trackMatches = state.track === 'all' ||
      (state.track === 'review' ? problem.progress?.due : problem.track === state.track);
    const difficultyMatches = state.difficulty === 'all' ||
      (state.difficulty === 'D4' ? Number(problem.difficulty.slice(1)) >= 4 : problem.difficulty === state.difficulty);
    const queryMatches = !query || [problem.id, problem.title, problem.topic]
      .some((value) => value.toLocaleLowerCase().includes(query));
    return trackMatches && difficultyMatches && queryMatches;
  });
}

function renderProblemList() {
  const problems = filteredProblems();
  elements.list.replaceChildren();
  elements.total.textContent = `${problems.length} / ${state.problems.length}`;
  if (problems.length === 0) {
    const empty = document.createElement('div');
    empty.className = 'empty-state';
    empty.textContent = state.track === 'review' ? '今天沒有到期的複習題目。' : '找不到符合條件的題目。';
    elements.list.append(empty);
    return;
  }

  for (const problem of problems) {
    const button = document.createElement('button');
    button.type = 'button';
    button.className = `problem-row${problem.id === state.selectedId ? ' is-active' : ''}`;
    button.dataset.problemId = problem.id;
    button.setAttribute('aria-label', `${problem.id} ${problem.title}, ${problem.difficulty}`);
    button.setAttribute('aria-current', problem.id === state.selectedId ? 'true' : 'false');

    const status = statusFor(problem);
    const statusNode = document.createElement('span');
    statusNode.className = `problem-status ${status.className}`;
    statusNode.textContent = status.symbol;
    statusNode.title = status.label;

    const copy = document.createElement('span');
    copy.className = 'problem-copy';
    const title = document.createElement('strong');
    title.textContent = problem.title;
    const id = document.createElement('span');
    id.textContent = `${problem.id} · ${topicLabel(problem.topic)}`;
    copy.append(title, id);

    const difficulty = document.createElement('span');
    difficulty.className = 'difficulty';
    difficulty.dataset.level = Number(problem.difficulty.slice(1)) >= 4 ? 'D4' : problem.difficulty;
    difficulty.textContent = problem.difficulty;
    button.append(statusNode, copy, difficulty);
    button.addEventListener('click', () => selectProblem(problem.id));
    elements.list.append(button);
  }
}

function inlineContent(text) {
  const fragment = document.createDocumentFragment();
  const parts = text.split('`');
  parts.forEach((part, index) => {
    if (index % 2 === 1) {
      const code = document.createElement('code');
      code.textContent = part;
      fragment.append(code);
    } else {
      fragment.append(document.createTextNode(part));
    }
  });
  return fragment;
}

function renderMarkdown(markdown) {
  const fragment = document.createDocumentFragment();
  const lines = markdown.replaceAll('\r\n', '\n').split('\n');
  let index = 0;
  let skippedTitle = false;

  while (index < lines.length) {
    const line = lines[index];
    if (!line.trim()) { index += 1; continue; }

    if (line.startsWith('```')) {
      const codeLines = [];
      index += 1;
      while (index < lines.length && !lines[index].startsWith('```')) {
        codeLines.push(lines[index]);
        index += 1;
      }
      index += 1;
      const pre = document.createElement('pre');
      const code = document.createElement('code');
      code.textContent = codeLines.join('\n');
      pre.append(code);
      fragment.append(pre);
      continue;
    }

    const heading = line.match(/^(#{1,3})\s+(.+)$/);
    if (heading) {
      index += 1;
      if (heading[1].length === 1 && !skippedTitle) { skippedTitle = true; continue; }
      const node = document.createElement(heading[1].length === 1 ? 'h2' : `h${heading[1].length}`);
      node.append(inlineContent(heading[2]));
      fragment.append(node);
      continue;
    }

    if (/^[-*]\s+/.test(line)) {
      const list = document.createElement('ul');
      while (index < lines.length && /^[-*]\s+/.test(lines[index])) {
        const item = document.createElement('li');
        item.append(inlineContent(lines[index].replace(/^[-*]\s+/, '')));
        list.append(item);
        index += 1;
      }
      fragment.append(list);
      continue;
    }

    if (/^\d+\.\s+/.test(line)) {
      const list = document.createElement('ol');
      while (index < lines.length && /^\d+\.\s+/.test(lines[index])) {
        const item = document.createElement('li');
        item.append(inlineContent(lines[index].replace(/^\d+\.\s+/, '')));
        list.append(item);
        index += 1;
      }
      fragment.append(list);
      continue;
    }

    const paragraphLines = [line.trim()];
    index += 1;
    while (index < lines.length && lines[index].trim() &&
           !/^(#{1,3})\s+|^[-*]\s+|^\d+\.\s+|^```/.test(lines[index])) {
      paragraphLines.push(lines[index].trim());
      index += 1;
    }
    const paragraph = document.createElement('p');
    paragraph.append(inlineContent(paragraphLines.join(' ')));
    fragment.append(paragraph);
  }
  elements.statement.replaceChildren(fragment);
}

function addMeta(text, value, accent = false) {
  const chip = document.createElement('span');
  chip.className = `meta-chip${accent ? ' accent' : ''}`;
  chip.append(document.createTextNode(`${text} `));
  const strong = document.createElement('strong');
  strong.textContent = value;
  chip.append(strong);
  elements.metadata.append(chip);
}

function formatReviewDate(progress) {
  if (!progress?.nextReviewDate) return '尚未排程';
  const date = new Date(`${progress.nextReviewDate}T00:00:00Z`);
  const formatted = new Intl.DateTimeFormat('zh-TW', { month: 'short', day: 'numeric', timeZone: 'UTC' }).format(date);
  return progress.due ? `${formatted} · 現在到期` : formatted;
}

function renderProgress(progress) {
  elements.attempts.textContent = progress?.attempts ?? 0;
  elements.accepted.textContent = progress?.acceptedAttempts ?? 0;
  elements.bestTime.textContent = progress?.bestTimeMs == null ? '—' : `${progress.bestTimeMs} ms`;
  elements.reviewDate.textContent = formatReviewDate(progress);
  elements.reviewRow.classList.toggle('is-due', Boolean(progress?.due));
  elements.verdict.className = 'verdict-pill neutral';
  elements.verdict.textContent = '尚未作答';
  if (progress?.lastVerdict === 'AC') {
    elements.verdict.className = 'verdict-pill accepted';
    elements.verdict.textContent = '✓ Accepted';
  } else if (progress?.lastVerdict) {
    elements.verdict.className = 'verdict-pill failed';
    elements.verdict.textContent = progress.lastVerdict;
  }
}

function starterFor(problem) {
  return `#include <iostream>\n\n// ${problem.id}: ${problem.title}\n// Target: ${problem.expectedTime}, ${problem.expectedSpace}\n\nint main() {\n    std::ios::sync_with_stdio(false);\n    std::cin.tie(nullptr);\n\n    // Write your solution here.\n\n    return 0;\n}\n`;
}

function saveCurrentDraft() {
  if (state.selectedId) localStorage.setItem(`structlab:draft:${state.selectedId}`, elements.editor.value);
}

async function selectProblem(problemId) {
  if (problemId === state.selectedId && elements.statement.childElementCount > 3) return;
  saveCurrentDraft();
  state.selectedId = problemId;
  renderProblemList();
  document.body.classList.remove('sidebar-open');
  elements.sidebarToggle.setAttribute('aria-expanded', 'false');
  elements.title.textContent = '載入題目…';
  elements.statement.replaceChildren();
  const loading = document.createElement('div');
  loading.className = 'skeleton-line wide';
  elements.statement.append(loading);

  try {
    const response = await fetch(`/api/problems/${encodeURIComponent(problemId)}`);
    if (!response.ok) throw new Error(`HTTP ${response.status}`);
    const { problem } = await response.json();
    elements.breadcrumbs.textContent = `題庫 / ${problem.track === 'advanced' ? '進階' : '基礎'} / ${topicLabel(problem.topic)}`;
    elements.index.textContent = problem.id.split('-')[0];
    elements.title.textContent = problem.title;
    elements.metadata.replaceChildren();
    addMeta('難度', problem.difficulty, true);
    addMeta('時間', `${problem.timeLimitMs} ms`);
    addMeta('記憶體', `${problem.memoryLimitMb} MB`);
    addMeta('複雜度', problem.expectedTime);
    renderMarkdown(problem.statement);
    renderProgress(problem.progress);
    elements.command.textContent = `npm run judge -- ${problem.id}`;
    elements.editor.value = localStorage.getItem(`structlab:draft:${problem.id}`) ?? starterFor(problem);
    history.replaceState(null, '', `/?problem=${encodeURIComponent(problem.id)}`);
    document.title = `${problem.id} · ${problem.title} · StructLab`;
  } catch (error) {
    elements.title.textContent = '題目載入失敗';
    elements.statement.textContent = `無法讀取 ${problemId}。請確認本機 Web server 仍在執行。`;
    console.error(error);
  }
}

let toastTimer;
function showToast(message) {
  clearTimeout(toastTimer);
  elements.toast.textContent = message;
  elements.toast.classList.add('is-visible');
  toastTimer = setTimeout(() => elements.toast.classList.remove('is-visible'), 1800);
}

async function copyText(text, successMessage) {
  try {
    await navigator.clipboard.writeText(text);
    showToast(successMessage);
  } catch {
    showToast('瀏覽器不允許存取剪貼簿');
  }
}

function setTrack(track) {
  state.track = track;
  document.querySelectorAll('[data-track]').forEach((button) => {
    button.classList.toggle('is-active', button.dataset.track === track);
  });
  renderProblemList();
}

async function initialize() {
  try {
    const response = await fetch('/api/problems');
    if (!response.ok) throw new Error(`HTTP ${response.status}`);
    const data = await response.json();
    state.problems = data.problems;
    renderProblemList();
    const requested = new URL(location.href).searchParams.get('problem');
    const initial = state.problems.find((problem) => problem.id === requested) ??
      state.problems.find((problem) => problem.id === 'F03-stack-array') ?? state.problems[0];
    if (initial) await selectProblem(initial.id);
  } catch (error) {
    const empty = document.createElement('div');
    empty.className = 'empty-state';
    empty.textContent = '題庫 API 無法連線。';
    elements.list.replaceChildren(empty);
    elements.title.textContent = '無法連線到本機題庫';
    elements.statement.textContent = '請重新啟動 npm run web。';
    console.error(error);
  }
}

elements.search.addEventListener('input', () => {
  state.query = elements.search.value;
  renderProblemList();
});

document.querySelectorAll('[data-track]').forEach((button) => {
  button.addEventListener('click', () => setTrack(button.dataset.track));
});

document.querySelectorAll('[data-difficulty]').forEach((button) => {
  button.addEventListener('click', () => {
    state.difficulty = button.dataset.difficulty;
    document.querySelectorAll('[data-difficulty]').forEach((item) => item.classList.toggle('is-active', item === button));
    renderProblemList();
  });
});

elements.editor.addEventListener('input', saveCurrentDraft);
elements.editor.addEventListener('keydown', (event) => {
  if (event.key === 'Tab') {
    event.preventDefault();
    const start = elements.editor.selectionStart;
    elements.editor.setRangeText('    ', start, elements.editor.selectionEnd, 'end');
    saveCurrentDraft();
  }
});

document.querySelector('#copy-command').addEventListener('click', () => copyText(elements.command.textContent, 'Judge 指令已複製'));
document.querySelector('#copy-link').addEventListener('click', () => copyText(location.href, '題目連結已複製'));
document.querySelector('#editor-toggle').addEventListener('click', () => document.body.classList.add('editor-open'));
document.querySelector('#close-editor').addEventListener('click', () => document.body.classList.remove('editor-open'));
elements.sidebarToggle.addEventListener('click', () => {
  const open = document.body.classList.toggle('sidebar-open');
  elements.sidebarToggle.setAttribute('aria-expanded', String(open));
});
document.querySelector('#mobile-scrim').addEventListener('click', () => {
  document.body.classList.remove('editor-open', 'sidebar-open');
  elements.sidebarToggle.setAttribute('aria-expanded', 'false');
});

document.addEventListener('keydown', (event) => {
  if (event.key === '/' && document.activeElement !== elements.search && document.activeElement !== elements.editor) {
    event.preventDefault();
    elements.search.focus();
  }
  if (event.key === 'Escape') {
    document.body.classList.remove('editor-open', 'sidebar-open');
    elements.sidebarToggle.setAttribute('aria-expanded', 'false');
  }
});

initialize();
