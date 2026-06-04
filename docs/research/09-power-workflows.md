# 09 — Power-User Workflows & Tutorials (Snagit → Ezsnagit)

**Purpose:** Understand HOW pros actually use Snagit end-to-end, and the keyboard/efficiency
tricks that matter, so the Ezsnagit (C++/Qt6) clone optimizes the right paths and the MVP
nails the *daily drivers* instead of rarely-touched features.

**Method:** WebSearch + WebFetch over TechSmith Academy/tutorials/support, TechSmith blog,
YouTube tutorial descriptions, and productivity/instructional blogs. Sources listed at bottom.

**Confidence flags:** Items marked *(verified TechSmith docs)* come from official tutorials.
Items marked *(community/secondary)* come from third-party blogs and may lag the current
version. Hotkeys are **Windows defaults** unless noted; many are user-rebindable, so treat
exact combos as defaults, not contracts. Version drift: Snagit ships ~yearly (2019→2026); some
features (Smart Move, Combine in Template, Smart Redact, Share Link) arrived in specific years —
flagged where relevant.

---

## The Snagit mental model (load-bearing for the clone)

Snagit is **two apps glued together**:

1. **Capture** — a lightweight always-resident capture engine (the orange crosshair UI,
   hotkeys, presets/OneClick). The goal here is *zero-friction acquisition*.
2. **Editor** — a canvas-based vector-annotation + light raster editor with a "tray" of recent
   captures and a searchable **Library**. The goal here is *annotate → assemble → share*.

Almost every pro workflow is: **acquire (capture) → annotate (editor) → assemble (template,
optional) → distribute (share destination)**. The clone should treat these four as the spine.

---

## 1. End-to-end workflows

### (a) Software documentation / step-by-step guides — THE flagship workflow

This is Snagit's headline use case (tech writers, IT, trainers, support, SOPs). Two distinct
sub-paths exist:

**Path A — Step Capture (automatic).** *(verified TechSmith docs)*
- Start from the **Presets menu → Step Capture** ("All-in-One" style preset).
- Snagit then **auto-screenshots on every click**. Each click produces one *step* containing:
  numbered identifier + (when detectable) the name of the clicked UI element + a screenshot
  with the **highlighted cursor centered** in the frame.
- A small floating toolbar shows the live step count; user clicks **Stop** when done.
- Snagit **auto-generates an editable guide**: title/header, one numbered step per click,
  screenshot per click, cursor highlight.
- Edit pass: rewrite step text (Quick Styles), recolor all step numbers at once, zoom/replace
  individual screenshots, reposition cursor, delete/duplicate/reorder/add sections.
- Export: **Share Link (Screencast), PDF, Word, PowerPoint** (Word/PPT send each step as a
  separate image + editable text). Mac adds individual-image and **Markdown** export.
- **Limit:** performance degrades past ~50 steps (Win) / ~100 (Mac); no hard cap.

**Path B — Manual capture + Step tool (deliberate).** *(verified TechSmith docs)*
- Capture each screen individually (often region/window capture), annotate, then place
  **Step tool** numbers (hotkey `1` in screen-draw; click to drop sequential numbers/letters).
- Add **arrows / callouts / text / Magnify / Blur** to direct attention.
- Apply **Simplify** to replace busy UI with clean placeholder shapes (SUI — "Simplified User
  Interface"), which makes docs version-proof and on-brand.
- Optionally **Combine in Template** into one deliverable (see §2).

**Features used:** Step Capture preset, Step tool, Quick Styles, Simplify (SUI), arrows/callouts,
Magnify, templates, Word/PPT/PDF/link export.
**Essential for clone:** capture, Step tool, arrows/callouts/text, export to PDF/image. Step
*auto-capture-on-click* is high-value but technically expensive (UI-element name detection +
cursor tracking) — strong differentiator, but MVP can ship manual Step tool first.

### (b) Bug reporting (QA / support / dev hand-off) *(verified + secondary)*
- Fast region/window capture of the broken state (hotkey-driven, no Editor open if using a
  copy-to-clipboard preset).
- Annotate: **arrow** to the defect, **callout/text** with repro notes, **Magnify** the error,
  **Step tool** if multi-step repro.
- **Blur / Smart Redact** to hide emails, tokens, account numbers, PII before sharing. **Smart
  Redact** auto-scans for sensitive data types (emails, phone, card numbers) and applies blur /
  pixelate / black bars. *Note: redactions flatten and cannot be un-done after flatten.*
- **Share Link** → one-click URL (Screencast / OneDrive / Google Drive) pasted into Jira/GitHub/
  Slack/Teams. Or direct **Share to Slack / Teams**.
- **Daily-driver core:** capture → arrow + text → blur → copy/share. This is the highest-frequency,
  lowest-ceremony workflow and should be the *fastest path in the whole app*.

### (c) Tutorial / training creation *(verified TechSmith docs)*
- Mix of **image step-guides** (above) and **video capture** (Shift+F9 start/pause,
  Shift+F10 stop).
- Customize the **webcam/camera** (shape, size, position) during recording.
- **Video → GIF** export for short looping demos *(community)*; **Create Video from Images**
  (Ctrl+Alt+V) to stitch stills.
- **Camtasia hand-off:** send Snagit video/recording straight to **Camtasia Editor** for serious
  timeline editing; also export to YouTube/Screencast. Snagit = quick capture; Camtasia = real
  edit. (Confluence integration not confirmed in current docs — flag as uncertain.)

### (d) Social media graphics *(verified + secondary)*
- **Fixed/known dimensions** capture (e.g., exactly 1080×1080 for Instagram) so output fits the
  target frame with no re-crop.
- Use **Image from Template** / **Combine in Template** for branded, consistent layouts; add
  titles/captions; resize sections via the divider lines.
- **Magic Wand** (color-select + tolerance + "Global Color Selection") to knock out plain
  backgrounds; **Stamps** for icons/arrows.
- Export to PNG/JPG; share to Slack/OneDrive/email/social.
- *(This is a real but lower-frequency use vs docs/bug-reporting — see ranking.)*

### (e) Extracting text / data — Grab Text (OCR) *(verified TechSmith docs)*
- **During capture:** Image tab → Selection dropdown → **Grab Text** → drag region → results
  dialog in Editor.
- **After capture:** right-click image on canvas → **Grab Text**.
- Output: editable text you can copy into Word/email/etc. Common real use: pull text out of PDFs,
  error dialogs, and non-selectable UI.
- Languages historically limited (EN/DE/FR/ES, partial JP); modern versions broadened. Flag:
  language coverage is version- and OS-dependent.
- Pros also use it to grab table-ish/data text out of screenshots quickly.

### (f) Scrolling capture of long pages / chats / spreadsheets *(verified + secondary)*
- **Auto scrolling (Scrolling Arrows):** All-in-One capture → orange directional arrows appear
  (down for long pages, right for wide sheets, center for full area) → Snagit auto-scrolls and
  stitches.
- **Manual / Panoramic capture:** select a region, then **Start** and scroll yourself while
  Snagit records + stitches frames. Used when auto-detect arrows never appear (custom apps,
  virtualized lists, chat windows).
- **Best-practice tips (matter for the stitcher):** set browser zoom to 100%; dismiss sticky
  headers / cookie banners / chat widgets (they corrupt stitching); scroll at a steady moderate
  pace (too fast = blurry overlap, too slow = visible seams).
- **Implication:** the *stitching engine quality* (overlap detection, fixed-header masking) is
  what separates a usable scrolling capture from a broken one. This is a hard but
  high-visibility feature.

---

## 2. Templates / "Create" — combining captures into one deliverable

*(verified TechSmith docs + community)*

- Entry points: **Create → Image from Template** (one image into a template) and
  **right-click tray/library → Combine in Template** (multiple images → one) — default hotkey
  **Ctrl+Alt+C**.
- Pick a pre-built template (side-by-side comparison, process, timeline, numbered steps).
- Drag images into desired order; add **titles/captions**; **resize sections** by dragging the
  blue divider lines; resize images/text via handles.
- Save customized templates (`.snagx`) with tags for reuse → enables team/brand consistency.
- Output is one shareable file (PNG/PDF) → email/Slack/OneDrive/etc.
- **Why pros love it:** turns N raw screenshots into one polished, numbered, branded artifact
  without a separate design tool. This is the "assemble" pillar.

---

## 3. Keyboard-driven efficiency (the part that makes pros fast)

**Capture / global** *(verified hotkeys tutorial; Windows / Mac)*
- Global capture: **Print Screen** (Win) / **Ctrl+Shift+C** (Mac)
- **Repeat Last Capture: Ctrl+Shift+R** — heavily used; re-fires the exact same capture
  (same region/mode) in one keystroke. A genuine daily driver for repetitive doc shots.
- Mac mode hotkeys: All-in-One **Ctrl+Shift+O**, Image **Ctrl+Shift+S**, Video **Ctrl+Shift+V**,
  Menu **Ctrl+Shift+E**.
- Video: Start/Pause **Shift+F9**, Stop **Shift+F10**; Create Video from Images **Ctrl+Alt+V**.
- Show/Hide magnifier **M**; Hide/Show Snagit **Ctrl+Shift+X** (per secondary source).

**Screen-draw / annotation single-key tools** *(verified)*
- Draw mode toggle **Ctrl+Shift+D**; Arrow **A**; Square **S**; **Step tool 1**; Magnifier **M**.

**Editor**
- Standard **Ctrl+Z / C / V / A**, Group **Ctrl+Shift+G**, Combine in Template **Ctrl+Alt+C**.

**Presets / OneClick — the real speed multiplier** *(verified)*
- **Presets** save a full capture config (mode + effects + destination) and can each carry a
  **custom hotkey**. Examples pros set up: "region → copy to clipboard, never open Editor,"
  "window → auto-border + auto-save PNG to folder," "Grab Text region," "scrolling full page."
- Create via *Add Preset → New preset from current settings → name → Add Hotkey*.
- **OneClick:** the screen-edge fly-out bar giving mouse access to capture modes/presets without
  the main window — for mouse-driven users who don't memorize hotkeys.

**Quick Styles & Favorites** *(verified)*
- **Quick Styles:** save a tool's exact look (color/font/shape/shadow/arrow style) as a reusable
  one-click style. Stops pros re-configuring an arrow every time.
- **Favorites (star):** pin most-used styles/tools from *any* category into one panel.
- **Customizable Editor toolbar:** add/remove/reorder tools, add spacers/groups. Pros prune it
  to ~5–8 tools they actually use.

**Takeaway:** the efficiency story is *presets + hotkeys + Quick Styles + Repeat-Last-Capture*.
These four convert Snagit from "a screenshot tool" into "muscle memory." The clone must treat
these as first-class, not settings buried in preferences.

---

## 4. Automation / integration / hand-off

*(verified TechSmith docs + support)*
- **Share destinations (direct outputs):** Word, PowerPoint, Excel, OneNote, SharePoint,
  OneDrive, Google Drive, Dropbox, Box, **Slack**, **Microsoft Teams**, Evernote, Screencast,
  YouTube (video). Presets can auto-route a capture to a destination with no clicks.
- **Office:** "Share to Word/PPT/Excel/OneNote" places the capture inline; for Step guides each
  step becomes a separate image + editable caption text.
- **Slack / Teams:** post a capture straight into a channel/DM to keep convo in one place.
- **Share Link:** one-click hosted URL (Screencast, or admin-configured OneDrive/Google Drive) —
  the dominant modern "paste a link into the ticket" flow.
- **Batch / "Apply to multiple":** batch-convert format and **batch-apply effects** to many files
  at once (borders, resize, watermark, format) — used for prepping image sets.
- **Camtasia hand-off:** Snagit video → Camtasia Editor for timeline editing. (Snagit deliberately
  stays "light capture"; heavy video editing is offloaded.)
- **Confluence:** *not confirmed* as a native destination in the docs reviewed — flag uncertain;
  in practice users paste Share Links or images. Don't assume a first-party connector.

---

## 5. Daily-driver vs rare — feature ranking (drives MVP scope)

Ranking synthesized from how often each feature appears across tutorials, "hidden features,"
and workflow articles, and which sit on the critical path of the top workflows (docs, bug
reports). *(Judgment call — flagged as opinion, not measured telemetry.)*

**Tier 1 — Daily drivers (MVP must nail these, must be fast):**
- Region / window / fullscreen **image capture** with crosshair + pixel-precise selection
- **Global capture hotkey** + **Repeat Last Capture (Ctrl+Shift+R)**
- **Arrow, Text, Callout, Box/Highlight** annotation with **Quick Styles**
- **Blur / redact** sensitive info
- **Copy to clipboard** + **save to file** (PNG)
- **Presets with hotkeys** (incl. copy-to-clipboard-no-editor)
- The **tray** of recent captures + basic **Library** (find a past capture)
- **Crop / resize / cut-out**

**Tier 2 — Frequent / high-value (fast-follow):**
- **Step tool** (manual numbering) and **Step Capture** (auto-on-click)
- **Scrolling capture** (auto + panoramic) + stitcher
- **Grab Text (OCR)**
- **Combine in Template / Image from Template**
- **Magnify**, **Simplify (SUI)**, **Smart Move** (move/edit objects & text in a screenshot)
- **Share to Slack/Teams** + **Share Link**
- **Favorites** + customizable toolbar
- **Video capture** (basic) + Office export

**Tier 3 — Rare / niche (post-MVP or skip):**
- **Smart Redact** (auto PII scan) — nice but Tier-1 manual blur covers the need first
- **Magic Wand** background removal; **Stamps** library
- **Video → GIF**, **Create Video from Images**, webcam shape customization
- **Batch effects / batch convert**
- **Tags** + smart library filters (by URL/app); **fixed social dimensions** presets
- Markdown export (Mac), Evernote/Box/Dropbox destinations, **Camtasia hand-off**
- Menu/Mission-Control/Panoramic edge cases

---

## 6. Workflow implications for Ezsnagit MVP

1. **Optimize the bug-report path above all.** capture → arrow + text → blur → copy/share-link
   is the highest-frequency loop. It should be doable in <5 seconds with hotkeys and never force
   opening a heavy editor (support a "copy to clipboard, no editor" preset on day one).

2. **Presets + hotkeys + Quick Styles are the product, not a setting.** Build the preset model
   (mode + effects + destination + hotkey) and Quick Styles early. Repeat-Last-Capture
   (Ctrl+Shift+R) is cheap to implement and disproportionately loved — ship it in MVP.

3. **Annotation engine must be vector/object-based** (movable arrows, editable text, re-styleable
   shapes), not flattened raster. Quick Styles + Favorites depend on tools being objects. Blur,
   however, should flatten on export (match Snagit's redaction semantics) so redaction is real.

4. **Step tool first, Step-Capture second.** Manual Step tool (drop sequential numbers) is easy
   and covers most doc needs. Auto-capture-on-click (UI element name + cursor tracking) is the
   marquee feature but is OS-integration-heavy — schedule it as a flagship fast-follow, not MVP.

5. **Templates = the "assemble" pillar.** Combine-in-Template is what turns a clone from
   "screenshot tool" into "documentation tool." Even a small set of layouts (side-by-side,
   numbered-steps, before/after) delivers most of the value. Make templates reusable/saveable.

6. **Scrolling capture is a differentiator but a deep engineering item.** Ship auto-scroll for
   standard windows/browsers first; panoramic/manual stitch second. Budget real time for the
   stitcher (overlap detection, sticky-header masking, 100%-zoom guidance) — a bad stitch is
   worse than no feature.

7. **Grab Text (OCR) is high-perceived-value, low-frequency.** Wire it to a mature OCR backend
   (e.g., Tesseract) rather than building OCR; expose both "capture region → text" and
   "right-click image → Grab Text."

8. **Sharing: prioritize clipboard + file + one hosted Share Link first.** Slack/Teams/Office
   direct connectors are valuable but each is integration work; a single robust "copy link"
   covers the dominant modern flow. Don't promise Confluence — unconfirmed even in Snagit.

9. **A searchable tray + Library is table stakes.** Pros rely on "find that screenshot from
   yesterday." Even a date/app-tagged local library materially improves the daily loop.

10. **Keep heavy video out of scope.** Snagit itself offloads serious editing to Camtasia. MVP
    should do quick image capture + (optionally) basic clip capture, and stop there.

---

## Sources

TechSmith official (tutorials / features / support):
- Step recorder feature — https://www.techsmith.com/snagit/features/step-recorder/
- Step Capture tutorial — https://www.techsmith.com/learn/tutorials/snagit/step-capture/
- Step tool tutorial — https://www.techsmith.com/learn/tutorials/snagit/step/
- 3 Ways to Get Faster with Snagit — https://www.techsmith.com/learn/tutorials/snagit/speed-up-snagit-workflow/
- Snagit Hotkeys Guide — https://www.techsmith.com/learn/tutorials/snagit/snagit-hotkeys/
- Preferences (Windows) — https://www.techsmith.com/learn/tutorials/snagit/preferences/
- 4 Ways to Start a Capture — https://support.techsmith.com/hc/en-us/articles/360030582052-4-Ways-to-Start-a-Capture-in-Snagit-Video
- Capture a Scrolling Area — https://www.techsmith.com/learn/tutorials/snagit/scrolling-capture/
- Grab & Edit Text — https://www.techsmith.com/learn/tutorials/snagit/edit-image-text/
- Grab/Edit Text best practices — https://support.techsmith.com/hc/en-us/articles/115002031592-Snagit-Grab-Text-Edit-Text-Best-Practices
- Extract text from an image (blog) — https://www.techsmith.com/blog/extract-text-from-image/
- Create Visual Guides with Templates — https://www.techsmith.com/learn/tutorials/snagit/image-from-template/
- Edit a Template — https://www.techsmith.com/learn/tutorials/snagit/modify-template/
- Combine Images feature — https://www.techsmith.com/snagit/features/combine-images/
- Combine Images into PDF — https://www.techsmith.com/snagit/features/combine-images-into-pdf/
- Smart Redact feature — https://www.techsmith.com/snagit/features/smart-redact/
- Hide Sensitive Information — https://www.techsmith.com/learn/tutorials/snagit/hide-sensitive-information/
- Share Captures — https://www.techsmith.com/learn/tutorials/snagit/share/
- Share to Microsoft Teams — https://www.techsmith.com/learn/tutorials/snagit/share-to-microsoft-teams/
- Snagit + Slack — https://www.techsmith.com/learn/tutorials/snagit/snagit-slack-app/
- Share to Word/PPT/Excel/OneNote — https://support.techsmith.com/hc/en-us/articles/360029399471-Share-to-Word-PowerPoint-Excel-or-OneNote-from-Snagit-Video
- Enterprise tool compatibility (Snagit/Camtasia) — https://support.techsmith.com/hc/en-us/articles/41687247503757
- Integrations — https://www.techsmith.com/integrations/
- Assets/Templates library — https://www.techsmith.com/snagit/assets/
- 25 Little-Known Snagit Features — https://www.techsmith.com/blog/snagit-hidden-features/
- Scrolling screenshot (blog) — https://www.techsmith.com/blog/scrolling-screenshot/

Community / instructional / secondary (treat as version-lagging):
- 6 Hidden Snagit Tricks — https://superstamps.co/snagit-6-tricks/
- Snagit scrolling screenshot guide (Captio) — https://www.captio.work/blog/snagit-scrolling-screenshot
- Snagit templates / infographics (DottoTech) — https://dottotech.com/snagit-templates/
- Use Snagit Templates to Streamline Documentation — https://steveanderson.com/2021/04/08/use-snagit-templates-to-streamline-documentation/
- Chris Menard — Combine images — https://chrismenardtraining.com/post/snagit-combine-images
- When Should You Use Snagit? (Startupik) — https://startupik.com/when-should-you-use-snagit/
- YouTube: Turbo Charge Snagit with Three Hotkeys — https://www.youtube.com/watch?v=kD8KXaNMJ3U
- YouTube: Create a Step-by-Step Guide with Step Capture — https://www.youtube.com/watch?v=lnZB-uXWa6Q

**Uncertainty flags:** Confluence as a native Snagit destination — *not confirmed* in reviewed
docs. Exact hotkeys are platform/version defaults and user-rebindable. Grab Text language
coverage and Step Capture step limits are version/OS-dependent. Tier rankings are analytical
judgment, not measured usage telemetry.
