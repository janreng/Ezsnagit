# Ezsnagit — Critical Review (Specs vs Research)

**Reviewer role:** senior product + engineering review.
**Date:** 2026-06-03.
**Scope reviewed:** specs `00`–`11` + all `ux-flows/*`, `README.md`, `ROADMAP.md`, research `01`–`09` (esp. the user/forum/competitor/review/workflow files 06–09).

**Core question:** do the specs + roadmap reflect what the research says actually matters to real users? **Short answer: the feature *inventory* is excellent and faithful to Snagit; the *prioritization* and the *non-functional / positioning* layer are where the plan diverges sharply from what research 06–09 say is make-or-break.** The roadmap was built bottom-up from Snagit's feature surface ("clone every panel, phase by module"), not top-down from the workflows and grievances the research identified. That is the central flaw.

---

## Executive summary — top 10 findings

1. **The "soul of the product" is scheduled last.** Research 09 §3/§6 states plainly that *presets + hotkeys + Quick Styles + Repeat-Last-Capture* "convert Snagit from a screenshot tool into muscle memory" and "are the product, not a setting." Yet the roadmap puts **Presets at P2, Quick Styles at "P3+/sau", hotkey customization at P12** (ROADMAP "P12 — Polish"; spec 10 §10.2 priority table). Repeat-Last-Capture is P2 with its hotkey "uncertain." This is the single biggest prioritization mismatch. **Fix: pull presets-with-hotkeys, Quick Styles, and Repeat-Last-Capture into the MVP band.**

2. **The highest-frequency workflow has no owner in the roadmap.** Research 09 §1(b)/§6 names the bug-report loop — *capture → arrow + text → blur → copy/share-link, doable in <5s, never opening the heavy editor* — as "the fastest path in the whole app." No spec or roadmap phase owns this end-to-end fast path. Its pieces are scattered across P1 (capture), P3 (annotation), P6 (share) and the critical "copy-to-clipboard, no editor" preset is buried in spec 01 §01.15 at P2. **Fix: define an explicit "Quick Capture / fast path" MVP milestone.**

3. **There is no non-functional-requirements spec, and NFRs are the actual competitive wedge.** Research 06 §2, 08 §5/§9 hammer that the *opportunity is "almost entirely about price/licensing model and resource footprint, not feature gaps"* (08 §1 verbatim). Sub-1s startup, small install, low idle CPU/RAM, correct mixed-DPI/multi-monitor, Library backup/restore, offline-first activation, honest licensing — **none of these have a spec.** They appear only as scattered one-liners. This is the most important missing document. **Fix: add `12-non-functional-requirements.md` and a short licensing/positioning section in README.**

4. **Mixed-DPI / multi-monitor — Snagit's #1 unmet bug — is copied, not fixed.** Research 08 §5 calls per-monitor-DPI a "years-old unmet Snagit pain point" and an explicit feature request (08 §7 Tier-2 #9). Spec 01 §01.6/§01.18 instead *reproduces* Snagit's limitation ("chỉ chụp một monitor tại một thời điểm", region-spanning "chưa phải headline feature — verify"). The plan adopts the competitor's weakness as its own spec. **Fix: make correct mixed-DPI capture geometry a day-one NFR, not a "verify later."**

5. **Library reliability/backup — a "trust is unforgivable to break" issue — is explicitly out of scope.** Research 06 §2.4 ("Snagit Library is Empty or Missing After Update", cloud-sync corruption) and 08 §9 #8 demand robust, cloud-sync-aware, update-surviving storage with easy backup/restore. Spec 06 has good atomic-write notes (§06.1) but then says backup/restore is "chưa scope ... cần spec riêng nếu làm" (06 Điểm chưa chắc). For the feature the research flags as a trust dealbreaker, "maybe later" is wrong. **Fix: promote backup/restore + sync-safety into spec 06 / the NFR spec at MVP.**

6. **Step-Capture (auto-on-click) is under-specified and Capture Info is mis-prioritized vs. its workflow value.** Research 06 §3 and 09 §1(a) make Step Capture the signature documentation feature and Capture-Info overlay the signature bug-report feature ("almost nobody knows it exists ... great for bug reports"). Step *auto-capture-on-click* exists only as a passing mention (spec 02 §02.7 covers the manual Step tool; auto-capture is acknowledged in research 09 §6 but has no spec section). Capture Info is parked at P5 (spec 03 §03.8) despite being core to the #2 workflow. **Fix: write a Step-Capture spec section; pull Capture Info earlier.**

7. **Several research-validated "delight"/differentiator features are entirely missing from the specs.** Not covered anywhere: **screen-freeze before selection** (research 07 §3/§10 Tier-1 #4 — "huge usability win"), **pinned/floating screenshots with click-through** (07 §3 CleanShot signature, 07 §10 Tier-2 #5), **after-capture task chain** (07 §1/§10 Tier-1 #3 — ShareX's headline differentiator), **Quick Access Overlay / capture history tray** (07 §10 Tier-2), **hide-desktop-icons / clean capture** (07 §3). These are exactly the items research 07 §10 marks "build early." **Fix: add them to specs + roadmap; at minimum screen-freeze and a lightweight after-capture chain belong in the differentiator tier.**

8. **Video (P10) is correctly scoped but mis-sequenced relative to OCR/Simplify, and is the heaviest risk in the program.** Research 06 §2.7, 08 §12, 09 §10 all say *keep video light or omit it*; the spec does scope it lightly (good). But the roadmap front-loads OCR (P8), Simplify/Smart Move (P9), and Video (P10) ahead of the polish/NFR work, when system-audio loopback, frame-accurate FFmpeg cut, and OpenCV inpaint are the three highest-risk items in the entire project. **Fix: treat Simplify/Smart Move/Edit-Text-in-place (OpenCV inpaint) as research-grade risk and consider deferring or de-scoping to "manual-only" for v1; sequence OCR (high value, lower risk) before them.**

9. **Flameshot — the only existing C++/Qt+GPL precedent — is in the research but absent from the specs.** Research 07 §4 calls Flameshot "the most important reference for us" (same stack: transparent fullscreen overlay, multi-monitor handling, Qt painting for annotations, global hotkeys). Spec 00 (architecture) and spec 01 (capture overlay) cite none of it. The architecture decisions (QGraphicsScene, Win32 RegisterHotKey) would benefit directly from referencing Flameshot's proven patterns and GPL pitfalls. **Fix: add a "technical precedents" note to spec 00 referencing Flameshot (study, don't copy) and ShareX's stitching/upload-chain behavior.**

10. **The roadmap's own "MVP = P0→P6" claim is internally inconsistent and over-claims "80% of daily value."** ROADMAP says "MVP dùng được = P0→P6 ... Đây là 80% giá trị hằng ngày." But by the research's own daily-driver ranking (09 §5 Tier-1), that MVP omits Repeat-Last-Capture polish, Quick Styles (deferred), and the fast-path preset, while *including* full Library tag/flag/website-filtering (06 P1) that research ranks Tier-3. The MVP definition is module-shaped, not workflow-shaped. **Fix: redefine MVP by the two flagship workflows (bug-report, doc-steps), not by completing modules P0–P6.**

---

## 1. Prioritization mismatches (detailed)

The roadmap is organized **by module/phase** (capture → editor → effects → library → share → scrolling → OCR → simplify → video → templates → polish). Research 09 is organized **by workflow and daily-driver frequency**. They disagree in concrete, fixable ways.

### 1.1 Buried-too-late (research says daily-driver / make-or-break)

| Feature | Research signal | Current placement | Verdict |
|---|---|---|---|
| **Hotkey customization** | 09 §3/§6: "presets + hotkeys ... are the product, not a setting" | **P12 (Polish)** — spec 10 §10.2, ROADMAP P12 | **Wrong.** Rebindable capture hotkeys + per-preset hotkeys belong in MVP. Print Screen alone (P1) is not enough; power users rebind because Print Screen collides with Win11 Snipping Tool (spec 01 §01.19 even documents the collision). |
| **Quick Styles** | 09 §3 (Tier-1 annotation), 06 §1.1 ("single highest-leverage thing") | **"P3+/sau"** — spec 02 priority table | **Wrong.** Quick Styles are why annotation is fast; flagged by both 06 and 09 as core. Should be P3 alongside the tools they style. |
| **Presets (mode+effect+destination+hotkey)** | 09 §3/§6 ("real speed multiplier"), 06 §1.7 | **P2** — spec 01 §01.15 | **Marginal.** P2 is acceptable but the *copy-to-clipboard-no-editor* preset specifically should be P1 (it's the fast-path enabler, 09 §6 #1). |
| **Repeat Last Capture** | 09 §3 ("heavily used ... genuine daily driver"), §6 ("cheap to implement and disproportionately loved — ship it in MVP") | **P2, hotkey "uncertain"** — spec 01 §01.19 | **Wrong.** Research explicitly says MVP. Trivial to build. Promote to P1. |
| **Capture Info overlay** | 09 §1(b), 06 §3 (core to bug-report workflow) | **P5** — spec 03 §03.8 | **Too late.** It's on the critical path of the #2 workflow (bug reports). Pull to MVP band. |
| **Library backup/restore + sync-safety** | 06 §2.4, 08 §9 #8 (trust dealbreaker) | **out of scope** — spec 06 Điểm chưa chắc | **Wrong** (see §3 / finding 5). |

### 1.2 Possibly-too-early / over-built (research ranks lower)

| Feature | Research signal | Current placement | Verdict |
|---|---|---|---|
| **Full Library filter set** (Applications/Websites/Tags/Flags/Favorites) | 09 §5 Tier-3 ("Tags + smart library filters"); 06 §1.6 loved but niche | **P1** — spec 06 §06.3/§06.4 | Search-by-name + date + type (P0) is enough for MVP daily loop; URL/app/tag filtering is fast-follow, not MVP-blocking. Reasonable but don't let it gate MVP. |
| **Advanced capture modes** (Menu P3, Multiple-Area P3, Freehand P4, Printer P5, Schedule/Interval P4) | 09 §5 Tier-3 / niche | spec 01 priority table | Correctly late — good. |
| **Templates at P1** | 09 §2/§6 #5 ("turns clone into a documentation tool") but 09 §5 Tier-2 | spec 08 §08.1 marks **P1**, ROADMAP puts Templates at **P11** | **Internal contradiction** (see §6.1). Templates are valuable but the P1-in-spec vs P11-in-roadmap mismatch must be resolved. |

### 1.3 Recommended re-ordered MVP (workflow-first)

Replace "MVP = complete P0–P6" with **MVP = the two flagship workflows fully fast**:

- **MVP-A (Bug-report loop, the highest-frequency path):** region/window/fullscreen capture + global hotkey (rebindable) + Repeat-Last-Capture + Arrow/Text/Callout/Box/Highlight with **Quick Styles** + Blur/redact + Crop + **copy-to-clipboard-no-editor preset** + save PNG + Recent tray + name/date search. **Plus Capture Info overlay.**
- **MVP-B (Doc-steps loop):** + manual **Step tool** + a small **Combine-in-Template** set (side-by-side / numbered-steps / before-after) + PDF/image export.
- **Fast-follow:** scrolling capture, Grab Text (OCR), share-link, full library filters, video.
- **Research-risk / defer:** Simplify, Smart Move, Edit-Text-in-place, system-audio video.

Crucially, **screen-freeze, magnifier/crosshair, and self-timer** (research 07 §10 Tier-1 #4, all cheap in Qt) should ride along in MVP-A as perceived-quality wins.

---

## 2. Missing / under-specified features

Cross-checking research 06 (hidden features), 07 (competitor ideas), 08 (requests), 09 (workflows) against specs 00–11:

**Entirely missing from specs (should be added):**
- **Screen-freeze before region select** — 07 §3/§10 Tier-1 #4, "huge usability win." Not in spec 01. *Add to spec 01.*
- **Pinned / floating screenshots** (float above all windows, opacity, click-through lock) — 07 §3/§10 Tier-2 #5. Not anywhere. *Add (new section or spec).*
- **After-capture task chain / pipeline** (effects → editor → OCR → save → upload → copy-link, user-composable) — 07 §1/§10 Tier-1 #3, the ShareX headline differentiator and a core part of the positioning thesis (07 §10). Specs only have static per-preset destination (01 §01.15) and a fixed share dropdown (07). *This is a strategic gap — add at least a lightweight destination-picker / post-capture chain.*
- **Quick Access Overlay / transient recent-capture tray** — 07 §10 Tier-2. Distinct from the Editor's Recent Tray (which requires the editor open). *Add.*
- **Hide-desktop-icons / clean capture** — 07 §3 (CleanShot signature). *Consider; cheap.*
- **Step-Capture (auto-screenshot-on-click)** — research 09 §1(a) Path A names it the flagship doc feature; spec 02 §02.7 only covers the *manual* Step tool. The auto-on-click engine (UI-element-name detection + cursor tracking) has no spec. *Add a Step-Capture section (flagship fast-follow per 09 §6 #4).*
- **Designer utilities** (pixel ruler, color picker/palette, protractor, crosshair magnifier, whiteboard) — 07 §10 Tier-3 #11 (PicPick differentiator, "cheap to build in Qt"). Roadmap-worthy. *Optional, note it.*
- **QR generation, custom HTTP uploader, FTP/SFTP destinations** — 07 §1/§10 Tier-2 #7. Spec 07 lists FTP at P2 but no generic custom-uploader (the ShareX power-feature). *Design upload layer as a pluggable interface from day one (research 07 §10 explicitly says so).*

**Under-specified (mentioned but thin):**
- **Crash-safety / autosave-recovery for video + large captures** — 08 §9 #9 ("Snagit loses captures on video-finalization crashes"). Spec 06 §06.1 has atomic-write for `.ezsnagx`; spec 05 §2.8 streams to disk; but there is no unified "recover unsaved capture after crash" requirement. *Make it an NFR.*
- **Share Link** — 09 §6 #8 says one robust hosted "copy link" covers the dominant modern flow. Specs correctly defer all cloud to P3 (07 §07.1), but Share-Link-as-a-concept is the single most-requested *modern* sharing path and is gated behind "cloud, P3." *Consider a minimal self-hostable/local-server link option earlier, or at least flag it as the priority cloud item.*
- **Onboarding / discoverability** — 06 §3 and 08 §6/§9 #11 stress that even TechSmith admits its best features go undiscovered; a clone should surface them (onboarding tips, searchable command palette). *No spec addresses discoverability at all.* Add to the UX/NFR plan.

---

## 3. Non-functional requirements the specs ignore

This is the most consequential gap. Research 06 §2/§7, 07 §10, 08 §5/§9 converge on the thesis that **the competitive opening is NFR + licensing, not features** (08 §1 verbatim: "The opportunity ... is almost entirely about price/licensing model and resource footprint, not feature gaps"). None of these are written down as requirements:

| NFR (research-backed) | Source | Spec status today |
|---|---|---|
| **Sub-1s cold start** | 08 §5, 06 §2.2 | Not stated anywhere |
| **Small installer / low idle RAM-CPU, no always-on CPU-hog helper** | 06 §2.2, 08 §5 | Not stated (only the README "lightweight" aspiration) |
| **Correct per-monitor mixed-DPI capture geometry** | 08 §5/§7 #9, 06 §2.3/§4.3 | **Contradicted** by spec 01 (copies Snagit's one-monitor limit) |
| **Library survives app updates; cloud-sync-aware; easy backup/restore** | 06 §2.4, 08 §9 #8 | Atomic-write only; backup/restore **explicitly out of scope** (06) |
| **Offline-first activation / no forced online sign-in / no phone-home** | 06 §2.6, 08 §7 #7 | Not stated (no licensing/activation spec at all) |
| **Honest, no-subscription, no-revocation licensing** | 06 §2.1, 08 §4 (loudest complaint everywhere) | Not stated as a product requirement |
| **Crash-safe large/long captures with autosave-recovery** | 08 §9 #9 | Partial (atomic write); no recovery requirement |
| **Progressive-disclosure UI (don't overwhelm)** | 08 §6/§9 #7 | Not stated |
| **Per-OS native feel (avoid "Windows-first port" complaint)** | 08 §3/§9 #10 | N/A for v1 (Windows-only) but should be noted for Qt |

**Recommendation: create `docs/specs/12-non-functional-requirements.md`** capturing the above as testable targets (e.g., "cold start < 1s on a 2020-era laptop", "installer < 60 MB excluding optional OCR/FFmpeg packs", "capture geometry correct on a 150%+100% dual-monitor setup", "Library auto-backup on every Nth write + one-click export/import", "no network call required to launch or capture"). These are *measurable* wins (08 §5) and they are the product's reason to exist (08 §1). The README/ROADMAP should lead positioning with **"own it, no subscription, works offline, lightweight"** (08 §9 #1) — currently the README says only "lightweight" as an aside and never mentions licensing.

---

## 4. Technical risk assessment

Genuinely hard parts for a C++/Qt clone, with effort/risk and suggested sequencing. The specs are honest about most of these (good "Điểm chưa chắc" sections), but the roadmap doesn't *sequence by risk*.

| Item | Spec | Effort | Risk | Notes / sequencing |
|---|---|---|---|---|
| **Scrolling-capture stitcher** (overlap detection, sticky-header masking, auto-scroll injection) | 01 §01.14, research 09 §1(f) | High | **High** | "A bad stitch is worse than no feature" (09 §6 #6). The auto-scroll message injection (`WM_MOUSEWHEEL`/SendMessage) plus end-of-content detection plus seam-free overlap matching is the hardest *capture* item. Ship **panoramic/manual stitch first** (deterministic), auto-scroll second. Study ShareX (GPL) for stitch approach (07 §1). |
| **Multi-monitor + mixed-DPI capture overlay** | 01 §01.3/§01.18 | Med-High | **High** | The transparent fullscreen overlay must be correct across monitors with different scale factors — the exact thing Snagit gets wrong (08 §5). This is foundational (P1) and currently under-specified; get it right before building features on top. Flameshot is the direct precedent (07 §4). |
| **System-audio loopback** (WASAPI) | 05 §2.3 | Med | **Med-High** | Qt Multimedia can't do loopback; needs a Win Core Audio platform layer + mixing mic+system via FFmpeg `amix`. Platform-specific, fiddly, but well-trodden. P9/P10. |
| **Frame-accurate video Cut/Concat** (FFmpeg keyframe handling) | 05 §3.2/§3.4 | Med | **Med** | Spec correctly notes re-encode-around-cut needed (05 §3.2). The edit-list-then-render model is sound. Main risk is A/V sync on concat with mismatched profiles. |
| **OCR accuracy (Tesseract vs ABBYY)** | 04 §04.13 | Low-Med | **Med** | Lower-risk because OCR is a solved library; accuracy will be *worse* than Snagit's ABBYY, esp. Japanese (spec acknowledges, 04 Điểm chưa chắc). Mitigate with upscale+contrast preprocessing and small-region selection UX. **Do this before Simplify/Smart Move** — higher value, lower risk. |
| **Simplify / SUI (OpenCV UI-element detection)** | 04 §04.1 | High | **High** | Plain OpenCV contour/morphology will be markedly less accurate than Snagit's purpose-built CV (spec admits, 04 Điểm chưa chắc). Detail-slider→kernel mapping needs empirical tuning with no known formula. **Research-grade risk.** Consider shipping manual-simplify-only for v1. |
| **Smart Move + auto-fill inpaint** (`cv::inpaint`) | 04 §04.8, 02 §02.12 | High | **Highest** | Inpainting on real UI backgrounds (gradients, textures) "có thể lộ" (will visibly fail) — the spec itself calls this "the key technical risk." Edit-Text-in-place (04 §04.12) compounds it (OCR + inpaint + font-match). **Strongly consider deferring past v1 or cutting** — it's the worst effort/payoff ratio in the program and the research (09 §5) ranks Smart Move only Tier-2/3. |
| **Video encode footprint** (FFmpeg bundling) | 05, 09 §03 | Low | **Med** | Technically fine; the *risk is to the NFR* — FFmpeg/OpenCV/Tesseract bundled together threaten the "small install" wedge (08 §5). Mitigate with **optional downloadable component packs** (already hinted in spec 11 §11.5) so the base install stays lean. |
| **Magnify object rendering** (composite-below sampling) | 02 §02.16 | Med | **Low-Med** | Needs render-on-demand of everything below; cacheable. Fine. |

**Suggested risk-first sequencing change:** lock down the **mixed-DPI/multi-monitor capture overlay** and **atomic/recoverable Library** as P1 foundations (they underpin everything and are the NFR wedge). Take the **scrolling stitcher** as a deliberate spike before committing its roadmap slot. Sequence **OCR before Simplify/Smart Move**. Treat **Smart Move/inpaint as optional research** — gate it behind a feasibility spike, don't put it on the critical path.

---

## 5. Competitive positioning

Research 07 §10 defines the open slot precisely: **"CleanShot X-quality UX + ShareX-style automation/destinations, cross-platform via Qt, cheaper than Snagit."** The specs deliver the *Snagit feature clone* but under-deliver on the two halves of that thesis:

- **CleanShot-quality UX:** the specs miss the exact interactions that make CleanShot the UX bar — screen-freeze, pinned/floating captures, Quick Access Overlay, magnifier polish (all flagged §2/§7 above). The capture flow (spec 01) reproduces Snagit's orange-toolbar flow faithfully but doesn't reach for the UX wins research 07 §10 marks "build early (Tier-1)."
- **ShareX-style automation:** the after-capture task chain and pluggable/custom upload destinations — ShareX's defining advantage and the second half of the positioning slot — are absent (spec 07 has only a static destination list + deferred cloud). Research 07 §10 Tier-1 #3 and Tier-2 #7 say design the upload layer as an interface from day one and offer a composable post-capture pipeline. The specs should commit to this.

**Flameshot:** Yes — it should absolutely be referenced as the technical precedent and is currently NOT in any spec. Research 07 §4 is unambiguous: same C++/Qt+GPL stack, "the best place to study Qt-level implementation patterns (transparent fullscreen overlay for region select, multi-monitor handling, Qt painting for annotations, system tray, global hotkeys)." Spec 00 (architecture) chooses exactly these mechanisms (QGraphicsScene, RegisterHotKey, frameless overlay) with no nod to the one existing project that has solved them in the same stack. **Add a "Technical precedents" subsection to spec 00**: Flameshot (Qt patterns, GPL = study-don't-copy), ShareX (stitching + upload-chain behavior, GPL), Greenshot (lightweight destination-picker UX). Note the GPL constraint explicitly so no one copies code.

**Positioning verdict:** the plan will produce a competent Snagit clone, but to occupy the *open slot* research 07 identifies it must add (a) a few CleanShot-grade capture interactions and (b) a ShareX-grade automation/destination layer, and (c) lead marketing with the licensing/footprint wedge (08 §1/§9). Right now it's positioned as "Snagit but `.ezsnagx`," not as the differentiated product the research points to.

---

## 6. Spec quality issues (inconsistencies / contradictions / scoping)

1. **Templates priority contradiction.** Spec 08 §08.1 marks Combine-Images **P1**; ROADMAP lists Templates as **P11**. A P1 feature cannot live in the 11th phase. Resolve (the research supports templates as an early differentiator for the doc workflow, 09 §6 #5 — lean toward earlier, but pick one).
2. **Two different priority vocabularies.** Specs 06/07/08/09/10 use **P0–P3** "MVP bands"; specs 01/02/03 and the ROADMAP use **P0–P12** "phases." "P3" means "annotation phase" in the roadmap but "deferred/cloud" in spec 07. This *will* mislead an implementer. **Normalize to one scheme** (recommend: roadmap phases P0–P12 everywhere, with an explicit "MVP" flag column).
3. **"MVP = P0→P6 = 80% of value" is unsupported.** ROADMAP closing note; contradicted by research 09 §5 (the daily-driver set spans into deferred Quick Styles/hotkeys). Either fix the MVP definition (§1.3) or drop the 80% claim.
4. **Spotlight tool-vs-effect is split-brained.** Spec 02 §02.18 (tool) and spec 03 §03.13 (effect) both define overlapping Spotlight behavior, with "clone chọn một mô hình" deferred in both. Two specs own one feature with no decision. **Pick one home.**
5. **Magnify vs Spotlight&Magnify duplication** (02 §02.16 object vs 03 §03.13 baked effect) is *intentional* and documented — acceptable, but make sure the editor UI doesn't surface two confusingly-similar "magnify" controls.
6. **Repeat-Last-Capture hotkey "uncertain" blocks an MVP feature.** Spec 01 §01.19 + spec 10 §10.2 both flag the Windows default key as unverified. For an MVP feature, just *pick a default* (e.g., Ctrl+Shift+R, matching Snagit Mac per research 09 §3) and move on — don't let "verify" gate it.
7. **Blur-as-object decision is correct but creates a redaction-safety subtlety.** Spec 02 §02.17 keeps Blur re-editable as an object, flattening on export. Research 09 §3 explicitly wants redaction to be *real* (flatten). The spec handles this (flatten on export) but should state loudly that **Solid-shape redaction is the only safe option** and that object-blur is reversible until flattened — a security note for the UI, not just the format.
8. **Email destination is unresolved.** Spec 07 Điểm chưa chắc correctly notes mailto can't attach; MAPI/COM needed. Fine to flag, but it's a P1 feature with an unsolved mechanism — decide before committing the P1.
9. **README status vs ambition mismatch.** README §"Trạng thái" says docs phase, code not started — consistent. But README's feature framing ("Capture → Editor → Library → Share") omits the very things research says win (hotkeys/presets/automation/licensing). Update the README pitch.
10. **No acceptance criteria / test hooks for the workflows.** Specs are feature-by-feature; there's no "the bug-report loop must complete in <5s with N keystrokes" style acceptance test, even though research 09 §6 gives exact targets. Add workflow-level acceptance criteria.

**General quality note (positive):** the per-spec "Điểm chưa chắc / cần verify" sections, the explicit vector-vs-raster architecture decision (spec 02), the headless-testable pure-logic split, and the honest sourcing in research 06–09 (flagging second-hand Reddit quotes, 404s) are genuinely high quality. The inventory faithfulness to Snagit is excellent. The problems are almost entirely about *ordering and the NFR/positioning layer*, not about feature understanding.

---

## 7. Concrete action list (prioritized)

**P0 — do before writing any code (planning fixes):**
1. **Add `docs/specs/12-non-functional-requirements.md`** — startup time, install size, idle footprint, mixed-DPI/multi-monitor capture correctness, Library backup/restore + sync-safety, offline-first activation, crash-recovery, honest-licensing. Make each a measurable target. (Findings 3, 4, 5.)
2. **Add a licensing/positioning section to README + ROADMAP** leading with "own it, no subscription, offline, lightweight." (Finding 3; research 08 §9 #1.)
3. **Normalize the priority scheme** across all specs to one vocabulary; add an explicit MVP flag. Resolve the **Templates P1-vs-P11** contradiction and the **Spotlight tool-vs-effect** ownership. (§6.1, §6.2, §6.4.)
4. **Redefine MVP by workflow** (MVP-A bug-report loop, MVP-B doc-steps loop) instead of "complete P0–P6"; add workflow acceptance criteria with the <5s/keystroke targets from research 09 §6. (Findings 1, 2, 10.)

**P1 — roadmap re-ordering (promotions/demotions):**
5. **Promote to MVP:** rebindable global hotkeys + per-preset hotkeys (from P12), **Quick Styles** (from "P3+"), **Repeat-Last-Capture** with a chosen default key (from P2/uncertain), **copy-to-clipboard-no-editor preset** (from P2), **Capture Info overlay** (from P5). (Finding 1, 6, §1.1.)
6. **Add to capture spec + MVP band:** **screen-freeze**, magnifier/crosshair polish, self-timer (cheap Qt wins, research 07 §10 Tier-1 #4). (Finding 7.)
7. **Make mixed-DPI/multi-monitor capture a P1 foundation**, and *delete* the copied "one monitor only" limitation framing in spec 01 §01.6/§01.18. (Finding 4.)
8. **Promote Library backup/restore + cloud-sync-safety into spec 06 at MVP** (remove "out of scope"). (Finding 5.)

**P2 — new specs / sections to write:**
9. **Step-Capture (auto-on-click) spec section** — the flagship doc feature, currently only manual Step tool exists. (Finding 6; research 09 §1(a).)
10. **After-capture task chain / destination-picker spec** (lightweight ShareX-style pipeline) + design the **upload layer as a pluggable interface** with a generic custom-HTTP/FTP uploader. (Finding 7, §5; research 07 §10.)
11. **Pinned/floating screenshots + Quick Access Overlay** spec section (CleanShot-grade UX). (Finding 7, §5.)
12. **Add "Technical precedents" subsection to spec 00** referencing Flameshot (Qt patterns, GPL), ShareX (stitch + upload chain), Greenshot (destination picker) — with the study-don't-copy GPL caveat. (Finding 9, §5.)

**P3 — risk management:**
13. **Spike the scrolling stitcher** before committing its slot; ship panoramic/manual first. (§4.)
14. **Sequence OCR (P8) before Simplify/Smart Move (P9)**; **gate Smart Move + inpaint behind a feasibility spike** and consider cutting Edit-Text-in-place from v1. (Finding 8, §4.)
15. **Keep the base install lean** via optional downloadable OCR/FFmpeg/OpenCV packs to protect the footprint NFR. (§4 last row; spec 11 §11.5 already hints this — make it a requirement.)
16. **Decide the Email mechanism** (MAPI vs COM) before locking its P1. (§6.8.)

---

*Prepared as a planning-stage review. The feature research and spec inventory are strong; the recommended changes are about (a) re-ordering toward the workflows and daily-drivers research 09 identifies, (b) writing down the NFR/licensing wedge research 06/08 say is the actual reason to build this, and (c) reaching past a literal Snagit clone toward the CleanShot+ShareX open slot research 07 maps.*
