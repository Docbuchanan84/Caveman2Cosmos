# Caveman2Cosmos Development Workspace Plan

Date: 2026-07-27
Workspace: `C:\Users\stacy\OneDrive\Documents\Caveman2CosmosGPT`

## 1. Executive recommendation

Use a true public GitHub fork:

- `origin` -> `https://github.com/Docbuchanan84/Caveman2Cosmos.git`
- `upstream` -> `https://github.com/caveman2cosmos/Caveman2Cosmos.git`
- Keep `master` synchronized with upstream and free of personal commits.
- Put personal work on `codex/<lowercase-hyphen-topic>` branches.
- Disable pushes to `upstream` locally and make `origin` the default push remote.

Reuse the unborn Git repository already in this workspace. Do not clone over it or initialize another repository.

Preserve the current SVN installation as a rollback copy before running `DevSetup.bat`. The Git development installation will then become the active exact-name `Mods\Caveman2Cosmos` directory.

No gameplay APIs, schemas, or assets will change during setup. The only repository additions will be this report and repository-local Codex documentation.

## 2. Current environment findings

- Operating system: Windows 11 Home Insider Preview, build 26220, 64-bit.
- Game distribution: GOG Civilization IV Complete 2.0.0.4.
- Beyond the Sword: `C:\GOG Games\Civilization IV Complete\Civ4\Beyond the Sword`
- Existing C2C: `C:\GOG Games\Civilization IV Complete\Civ4\Beyond the Sword\Mods\Caveman2Cosmos`
- Existing C2C is a SourceForge SVN working copy at revision 11678.
- TortoiseSVN reports no modified versioned files or mixed revisions.
- Unversioned installation data consists of `UserSettings`, `Autolog`, and `memory.log`.
- The installed changelog identifies v45.BETA.8837 dated 2026-07-22.
- Civ IV user data is under `C:\Users\stacy\OneDrive\Documents\My Games\Beyond The Sword`.
- Cache and profiles are under `C:\Users\stacy\AppData\Local\My Games\Beyond the Sword`.
- GitHub CLI is authenticated as `Docbuchanan84`.
- No existing empty or C2C-related personal repository was found before execution.

Installed development tools include Git 2.49.0, Git LFS 3.6.1, GitHub CLI 2.92.0, Python 3.13.3, PowerShell 5.1 and 7.6.4, Visual Studio 18 Insiders, Visual Studio Build Tools 2022 with v142/v143, Windows SDK 10.0.22621.0, TortoiseSVN 1.14.5, and Java 8 JRE.

The workspace is inside OneDrive. It must remain a real directory because OneDrive does not support syncing through symbolic links or junctions. Pin the workspace locally and verify that no repository files are offline before building.

## 3. Authoritative project sources

- Development repository: <https://github.com/caveman2cosmos/Caveman2Cosmos>
- Developer Guide: <https://github.com/caveman2cosmos/Caveman2Cosmos/wiki/Developer-Guide>
- C++ Style Guide: <https://github.com/caveman2cosmos/Caveman2Cosmos/wiki/CPP-Style-Guide>
- SVN player instructions: <https://github.com/caveman2cosmos/Caveman2Cosmos/wiki/Using-SVN>
- OneDrive restrictions: <https://support.microsoft.com/en-US/onedrive/restrictions-and-limitations-in-onedrive-and-sharepoint>

All URLs were checked on 2026-07-27.

The authoritative development branch is `master`. At investigation time, `master` and the protected `release` branch both pointed to `4f6bf1b0a132e5aea6028f2a78ebd2a2bd1708e5`.

The repository contains source, tools, normal assets, and unpacked art without submodules or Git LFS rules. `DevSetup.bat` creates the runtime junction installation, builds a Release DLL, and generates Visual Studio debugging settings. DLL compilation uses the tracked FASTBuild executable and legacy dependencies extracted from `Tools\deps.exe`.

SourceForge SVN is a generated player/tester distribution exported from the GitHub release pipeline. It is not the development source.

## 4. Recommended repository architecture

Create `Docbuchanan84/Caveman2Cosmos` as a public fork. Configure:

```powershell
git remote add origin https://github.com/Docbuchanan84/Caveman2Cosmos.git
git remote add -t master upstream https://github.com/caveman2cosmos/Caveman2Cosmos.git
git remote set-url --push upstream DISABLED
git fetch --prune origin
git switch -C master --track origin/master
git config remote.pushDefault origin
git config branch.master.pushRemote origin
git config branch.master.rebase true
git config pull.rebase true
git config fetch.prune true
```

Update the clean base with:

```powershell
git fetch --prune upstream
git switch master
git merge --ff-only upstream/master
git push origin master
```

Create personal work with:

```powershell
git switch -c codex/<topic> master
```

Use `git revert` for published mistakes. Do not rewrite published history or push to upstream.

## 5. Existing installation protection and migration plan

Before changing the active mod:

1. Confirm `Civ4BeyondSword.exe` is not running.
2. Recheck the SVN revision and modification state with TortoiseSVN `SubWCRev.exe`.
3. Back up `UserSettings`, `Autolog`, `memory.log`, and `CivilizationIV.ini` to `D:\C2C-Backups\2026-07-27-r11678`.
4. Create and verify a SHA-256 manifest for the backup.
5. Rename the existing mod to `Caveman2Cosmos_SVN_r11678_2026-07-27` on the same volume.
6. Verify that the renamed SVN working copy remains readable and at revision 11678.
7. Only then run `DevSetup.bat`.

Do not copy `.svn`, packed FPKs, DLL/PDB output, generated changelogs, logs, caches, saves, `memory.log`, or `UserSettings` into Git.

Start the development installation with fresh user settings. Keep the old settings available for later selective comparison.

Rollback:

1. Close the game.
2. Verify the active development shell and every junction target.
3. Rename the active development shell to a timestamped inactive name; do not delete it.
4. Rename the preserved SVN directory back to `Caveman2Cosmos`.
5. Recheck its SVN revision before launching.

## 6. Required tools and dependencies

No immediate installation is required. Use the installed Git, GitHub CLI, TortoiseSVN inspection utilities, repository FASTBuild toolchain, and Python 3 utilities.

Test `Sources\C2C (VS2019).sln` in Visual Studio 18 Insiders. Install Visual Studio 2022 Community with Desktop Development with C++ and v142 support only if solution loading or 32-bit debugging fails.

## 7. Ordered implementation plan

1. Save this report.
2. Pin the workspace locally and verify disk capacity.
3. Create and verify the public GitHub fork.
4. Configure `origin` and push-disabled `upstream`, fetch, and check out `master`.
5. Validate repository integrity.
6. Create `codex/repository-onboarding`.
7. Back up and rename the SVN installation.
8. Run `DevSetup.bat` against the GOG Beyond the Sword `Mods` directory.
9. Verify junctions, build dependencies, DLL/PDB, and Visual Studio settings.
10. Run XML, DLL, launch, log, and fresh-game validation.
11. Create the Codex documentation.
12. Commit and push documentation only to `origin`.
13. Do not create an upstream pull request.

## 8. Codex knowledge and instruction system

Create a concise root `AGENTS.md` with authoritative build/test commands, Git rules, installation and save protection, generated-file boundaries, domain validation, and the warning that `_BootDLLCheck.bat` can terminate and restart Civ IV.

Create:

- `Docs/Codex/README.md`
- `Docs/Codex/ARCHITECTURE.md`
- `Docs/Codex/WORKFLOWS.md`
- `Docs/Codex/DOMAINS.md`
- `Docs/Codex/COMPATIBILITY.md`
- `Docs/Codex/SOURCES.md`
- `Docs/Codex/DECISIONS.md`
- `Docs/Codex/GLOSSARY.md`

Classify claims as code-confirmed, official documentation, historical documentation, maintainer guidance, unverified community guidance, or personal-fork decision.

## 9. Repository onboarding task

Pin documentation to the checked-out commit, map XML/Python/C++/AI/UI/assets/runtime flows, document save and multiplayer hazards, identify generated files, cite code paths, reuse official documentation rather than duplicating it, and make documentation-only changes.

## 10. Build, launch, and test strategy

Baseline gates:

- Clean Git status before setup.
- Correct junction targets.
- Successful Release DLL build and deployment.
- Successful `Tools\XmlValidator.exe -a`.
- Successful launch to the main menu.
- Fresh test game, several completed turns, save to a new filename, and reload.
- No new Python or XML errors.
- No generated files in Git status.

Domain-specific validation:

- XML/localization: validator, boot, text-key checks.
- Python/UI: Python logs, affected interaction, save/reload.
- C++/AI: Release and Assert builds, focused scenario, save/reload, assert/crash logs.
- Art/audio: edit `UnpackedArt`, validate generated FPK output without committing it.
- Save compatibility: copied saves only.
- Multiplayer: identical commit and DLL hashes, checksum/OOS comparison.
- Performance: identical save and turn workloads with profiling configurations.

## 11. Risk register

| Risk | Level | Mitigation |
|---|---:|---|
| `DevSetup.bat` replaces the active exact-name C2C directory | Critical | Back up and manually rename SVN first |
| OneDrive conflicts or offline files | High | Pin locally, verify hydration, keep GitHub as tracked backup |
| Unsafe junction removal | High | Resolve every target first and rename for rollback |
| Accidental upstream push | High | Push-disabled upstream and origin default |
| Save incompatibility | High | Test copies and preserve SVN |
| Legacy dependency quarantine | Medium | Use tracked binaries and inspect Defender events |
| Disk/sync pressure | Medium | Maintain at least 15 GiB free |
| Generated files committed | Medium | Review ignored/generated boundaries before commits |
| Visual Studio incompatibility | Medium | Validate first; install VS 2022 only on failure |
| Incomplete automated Python tests | Medium | Require in-game Python/UI smoke tests |

## 12. Decisions

Approved:

- True public GitHub fork under `Docbuchanan84`.
- Preserved SVN rollback installation.
- Active Git development installation at the exact C2C mod path.
- Current OneDrive workspace retained with mitigation.
- No automatic migration of old user settings.
- No upstream pull request or gameplay change during setup.

## 13. Execution instruction

Execute this plan in order. Verify every backup before renaming the active installation. Never delete the preserved installation, saves, settings, or an unverified reparse point. Keep `master` clean, push only to `origin`, and stop if a discovered path, revision, remote, or setup-script behavior differs materially from this report.
