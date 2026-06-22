---
type: security_policy
status: active
priority: p1
updated: 2026-06-21
context_policy: retrieve_only
owner: project
---

# Security And Permission Boundary

## Scope

The initial Judge is a local, single-user learning tool. It executes the repository owner's C++ code.
It is not a secure multi-tenant sandbox and must not be exposed as a public code-execution service.

## Principles

- Least privilege and workspace-bounded file access.
- No network access is required for judging.
- Learner executables run in child processes with timeout and bounded output.
- Temporary files use a dedicated directory and are removed only after their resolved path is verified.
- Shell arguments are passed as structured argument arrays; untrusted strings are not concatenated into shell commands.
- Secrets, tokens, and credentials never belong in problem metadata, logs, fixtures, or documentation.

## Trust Boundaries

| Boundary | Input | Required protection |
|---|---|---|
| Problem catalog | Repository metadata and test files | Schema validation and path containment |
| Compiler runner | Source paths and compiler flags | Allowlisted flags and structured arguments |
| Executable runner | Learner binary and test input | Timeout, output bound, working-directory isolation |
| Progress storage | Local verdict and timing records | Atomic write and schema validation |
| Local Web reads | Loopback HTTP requests | Static allowlist, known problem IDs, no repository mutation |
| Compile check | Browser C++ source | Loopback-only POST, one compile at a time, 128-KiB source cap, fixed no-shell flags, 20-second timeout, 64-KiB diagnostics cap, no binary execution |

## Approval-Gated Changes

- Network-accessible code execution.
- Running submissions from untrusted users.
- Writes or deletes outside the repository-controlled runtime directory.
- Permission elevation, credentials, telemetry, plugin loading, or dynamic command templates.
- Adding a sandbox or container dependency.

## Required Security Tests

- Paths containing spaces, Unicode, and traversal segments.
- Compiler and executable timeout.
- Excessive stdout/stderr.
- Failed compile and crashed process cleanup.
- Symlink or junction escape where supported.
- Malformed metadata and unexpected file types.
- Oversized/malformed compile requests, missing compiler, compile timeout, and diagnostic truncation.

## Automated Checks

- `npm run security:scan`
- `npm run docs:refresh`
- Runner-specific C++ and loopback Web integration tests
