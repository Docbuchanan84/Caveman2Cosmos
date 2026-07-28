# Codex development index

Documentation baseline: `4f6bf1b0a132e5aea6028f2a78ebd2a2bd1708e5`
(upstream `master`, checked 2026-07-27).

This directory is a navigation and evidence layer for development work. It does not
replace the code, the official developer guide, or change-specific investigation.

## Evidence classes

Every factual claim that is not self-evident from a cited path should use one of these
labels:

- **[CODE]** directly confirmed in the pinned repository revision.
- **[OFFICIAL]** current official C2C repository or wiki documentation.
- **[HISTORICAL]** older official/project material retained for context.
- **[MAINTAINER]** guidance attributed to a maintainer but not encoded as a rule.
- **[UNVERIFIED]** community guidance that still needs reproduction.
- **[LOCAL]** a decision or observed state of this personal fork/workstation.

If code and prose disagree, record the disagreement and follow the code for current
behavior. Revalidate path citations after an upstream sync.

## Map

- [Architecture](ARCHITECTURE.md): runtime layers and data flow.
- [Workflows](WORKFLOWS.md): Git, setup, build, launch, validation, and rollback.
- [Domains](DOMAINS.md): where to start for common change types.
- [Compatibility](COMPATIBILITY.md): saves, multiplayer, performance, caches, and
  generated files.
- [Sources](SOURCES.md): source register with dates and classifications.
- [Decisions](DECISIONS.md): append-only personal-fork decisions.
- [Glossary](GLOSSARY.md): project terms.

## Evidence policy

A completed change report should state the starting SHA, changed paths, commands and
exit results, runtime evidence, generated/ignored files observed, and any gate that was
not run. “Build succeeded” and “game launched” are different claims and require
different evidence. **[LOCAL]**
