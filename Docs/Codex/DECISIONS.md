# Personal-fork decisions

Append entries; do not rewrite old decisions to make history look cleaner.

## 2026-07-27 — Public fork and remote safety

- Status: accepted.
- Classification: **[LOCAL]**.
- Decision: use `Docbuchanan84/Caveman2Cosmos` as `origin`, the official repository
  as fetch-only `upstream`, clean `master`, and `codex/<topic>` feature branches.
- Reason: preserves upstream synchronization while making accidental official pushes
  fail locally.

## 2026-07-27 — Active Git install with preserved SVN rollback

- Status: accepted.
- Classification: **[LOCAL]**.
- Decision: preserve SVN revision 11678 as
  `Caveman2Cosmos_SVN_r11678_2026-07-27`, keep a verified local-settings backup at
  `D:\C2C-Backups\2026-07-27-r11678`, and make the exact-name active mod the
  junction-based Git development shell.
- Reason: provides an immediate rollback without mixing SVN metadata or generated
  player assets into Git.

## 2026-07-27 — Fresh baseline settings

- Status: accepted.
- Classification: **[LOCAL]**.
- Decision: do not automatically migrate `UserSettings`, logs, cache, or saves into
  the development installation.
- Reason: separates code/setup validation from historical local configuration.

## 2026-07-27 — Documentation-only onboarding

- Status: accepted.
- Classification: **[LOCAL]**.
- Decision: the onboarding branch changes only setup/Codex documentation. No gameplay
  API, schema, or asset change is part of onboarding.

## 2026-07-27 — Visual Studio 18 C++ workload

- Status: completed after an interactive installer prompt.
- Classification: **[LOCAL]**.
- Decision: retain Visual Studio 18 Insiders and add its Desktop development with C++
  workload instead of installing Visual Studio 2022 Community side-by-side.
- Evidence: Visual Studio Setup reported `Completed install` with an empty error log;
  MSVC `14.50.35717` is present; `C2C (VS2019).sln` then opened in a responsive
  Visual Studio 18 Insiders window and closed normally.
- Consequence: the repository's FASTBuild/dependency path remains authoritative; the
  IDE workload is for editing and debugging.
