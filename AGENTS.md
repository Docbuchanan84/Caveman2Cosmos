# Caveman2Cosmos repository instructions

These instructions apply to the whole repository. The documentation baseline is commit
`4f6bf1b0a132e5aea6028f2a78ebd2a2bd1708e5`; re-check code paths after rebasing.

## Evidence and scope

- Label durable documentation claims using the evidence classes in
  `Docs/Codex/README.md`.
- Prefer code-path citations over recollection. Keep volatile operational detail in
  `Docs/Codex`, not in this file.
- Protect saves and the preserved SVN installation. Never test against the only copy
  of a save, copy old settings into a baseline automatically, or recursively remove an
  unverified reparse point.

## Git rules

- `origin` is the personal public fork. `upstream` is the official repository and has
  a deliberately disabled push URL.
- Keep `master` equal to `upstream/master` and free of personal commits.
- Develop on `codex/<lowercase-hyphen-topic>` branches, rebase them on
  `upstream/master`, and push only to `origin`.
- Use `git revert` for published mistakes. Do not rewrite published history without
  explicit approval.

## Build, launch, and validation

Run commands from the repository root:

```powershell
cmd /c Tools\_MakeDLL.bat Release build deploy
cmd /c Tools\_MakeDLL.bat Assert build
Tools\XmlValidator.exe -a
cmd /c LaunchC2C.bat
```

Before launch, verify that `Tools\mods_directory.txt` names the real Beyond the Sword
`Mods` directory and that the active shell's `git_directory.txt` points here.

`Tools\_BootDLLCheck.bat` may forcibly terminate and restart Civilization IV when the
DLL changed. Never run it while an unsaved game is open.

## Generated and local-only boundaries

Do not commit `Build`, DLL/PDB files, generated FPKs, logs, `UserSettings`,
`CHANGELOG.md`, `mods_directory.txt`, Visual Studio user files, or
`Assets/fpklive_token.txt`. Make art-source changes under `UnpackedArt/art`; FPKLive
produces runtime packs.

## Minimum domain gates

- XML/localization: XML validator, boot, affected text key/screen.
- Python/UI: boot, Python logs, affected interaction, save/reload.
- C++/AI: Release and Assert builds, focused scenario, save/reload, assert/crash logs.
- Art/audio: generated-pack run plus visual or audible inspection.
- Save/network-sensitive work: copied saves only; identical commits and DLL hashes for
  multiplayer; compare checksum/OOS logs.

See `Docs/Codex/README.md`, `Docs/Codex/WORKFLOWS.md`, and
`Docs/Codex/COMPATIBILITY.md` for the evidence-backed details.
