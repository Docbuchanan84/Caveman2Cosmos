# Workflows

## Repository synchronization

```powershell
git fetch --prune upstream
git switch master
git merge --ff-only upstream/master
git push origin master
git switch -c codex/<topic> master
```

`origin` is the personal fork; `upstream` is read-only by local configuration.
Personal commits never belong on `master`. **[LOCAL]**

Before work, record `git rev-parse HEAD` and `git status --short --branch`. Before a
commit, run `git diff --check`, inspect ignored output, and stage explicit paths.
**[LOCAL]**

## Development installation

`DevSetup.bat` calls `Tools/Install.bat DevSetup`, builds/deploys Release through
`Tools/_MakeDLL.bat`, and runs `Tools/UpdateVSUserFile.bat`. The install script creates
an exact-name `Mods/Caveman2Cosmos` shell, junctions `Assets`, `PrivateMaps`,
`PublicMaps`, and `Resource`, copies `Caveman2Cosmos.ini`, and writes
`git_directory.txt`. **[CODE]**

Do not run setup over an unpreserved installation. Resolve each reparse target before
moving or removing an active shell. **[LOCAL]**

## Build

```powershell
cmd /c Tools\_MakeDLL.bat Release build deploy
cmd /c Tools\_MakeDLL.bat Assert build
cmd /c Tools\_MakeDLL.bat Profile build
cmd /c Tools\_MakeDLL.bat ProfileExtra build
```

`Tools/InstallDeps.bat` extracts the bundled legacy toolchain when needed.
`Sources/fbuild.bff` defines Debug, Release, Assert, Profile, ProfileExtra, Testing,
and FinalRelease configurations. Visual Studio is an editor/debugger front end; these
commands are the authoritative build route. **[CODE]**

## Static validation and launch

```powershell
Tools\XmlValidator.exe -a
cmd /c LaunchC2C.bat
```

The repository workflow `.github/workflows/commit.yml` runs the same XML validator;
its Python lint/test jobs are currently commented out. In-game Python/UI smoke
testing is therefore mandatory for Python-facing changes. **[CODE]**

At launch, the development DLL can run FPKLive and `_BootDLLCheck.bat`.
`_BootDLLCheck.bat` calls `taskkill /F` and relaunches Civ IV when the built DLL
differs from the staged DLL. Never invoke it with an unsaved game. **[CODE]**

For a baseline smoke test: reach the main menu, start a fresh disposable game, play
several turns, save under a new test name, reload, exit normally, and compare new
`PythonErr.log`, `xml.log`, assert, crash, and OOS output with the pre-test state.
**[LOCAL]**

## Debugging

Open `Sources/C2C (VS2019).sln` after `UpdateVSUserFile.bat` has generated the local
debugger settings. Reproduce on a copied save, retain exact SHA/build configuration,
and capture the first error rather than only downstream failures. **[LOCAL]**

## Rollback on this workstation

1. Close Civilization IV.
2. Verify the active shell's `git_directory.txt` and all junction targets.
3. Rename the active development shell to a timestamped inactive name; do not delete
   it.
4. Rename `Caveman2Cosmos_SVN_r11678_2026-07-27` back to
   `Caveman2Cosmos`.
5. Recheck SVN revision 11678 before launch.

The verified local backup is `D:\C2C-Backups\2026-07-27-r11678`. **[LOCAL]**
