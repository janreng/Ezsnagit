# Snagit — Image Effects, Transforms, Simplify, and Grab Text (OCR)

> Research notes for the Snagit-Clone project. Sourced primarily from official TechSmith
> documentation (techsmith.com/learn tutorials and support.techsmith.com) plus reputable
> secondary sources. Reflects the current Snagit (Windows 2026.x / Mac 2025.x) generation.
>
> **Scope:** Effects, canvas/image transforms, the Simplify tool, Grab Text / Edit Text OCR,
> Spotlight & Magnify, Watermark, and Smart Move. Windows vs Mac differences are flagged
> throughout.
>
> **Important caveats / uncertainties** (do not treat the following as fully confirmed):
> - Snagit's tool sets and panels differ notably between **Windows** and **Mac**, and TechSmith
>   ships frequent updates that move/rename options. Items below marked *(uncertain)* could not be
>   confirmed against a current, authoritative page during this research pass.
> - "Effects are only available for images" in Snagit — video must be exported to Camtasia for
>   annotation/effects. All effects described here apply to image captures.
> - As of **May 1, 2026**, TechSmith shipped updates with a renewed third‑party OCR license so that
>   Grab Text and other OCR features keep working (the previous ABBYY OCR license expired). Older
>   Snagit builds may lose OCR functionality until updated.

---

## 1. Effects model and how effects are applied

Snagit groups image enhancements under an **Effects** panel/button in the Editor.

- **Effects are image-only.** They are applied to the whole canvas (the flattened image), not to
  individual annotation objects, with the partial-selection exception noted below.
- **Quick Effects:** Saved combinations of effects you can re-apply with one click.
- **Apply flow differs by OS:**
  - **Windows:** Open Effects, choose an effect, adjust its properties, then click **Apply**.
  - **Mac:** Click an effect to toggle it on, then click the **settings/gear icon** to customize.
    New custom effects appear at the bottom of Quick Effects; click the **+** to save one or the
    **star** to add to Favorites.
- **Apply an effect to only part of an image (Windows only):** Click the **Selection** tool, drag a
  selection on the canvas, then apply the effect — it affects only the selected region. (Mac does
  not support selection-scoped effects in the Effects panel.)
- **Removing effects:**
  - **Windows:** `Edit > Undo` repeatedly.
  - **Mac:** Click the blue (active) effect name to toggle it off, or click the **No Effects**
    quick style to clear all.
- **Cross-platform note:** Mac saves layered work in **`.snagx`**, which does not open on Windows.
  Export to PNG/JPG/etc. to preserve a baked-in effect across platforms.

### Effect availability matrix (Windows vs Mac)

| Effect | Windows | Mac | Notes |
|---|---|---|---|
| Border | Yes | Yes | |
| Edges (drop shadow, torn, faded, wave, saw, shark tooth, beveled) | Yes | Yes | Per-side control; Windows adds shadow color/location |
| Shadow | Yes | Yes | Mac adds opacity/offset/blur controls |
| Perspective | Yes | Yes | |
| Page Curl | Yes | No | |
| Reflection | No | Yes | Mirror reflection below image |
| Fade | No | Yes | Fades image edges |
| Spotlight and Magnify | Yes (effect) | No (use Spotlight **tool** instead) | |
| Watermark | Yes | No (use logo/stamp or opacity workaround) | |
| Color Adjustment (brightness/contrast/hue/saturation/gamma) | Yes | Yes | |
| Filters (soften, sharpen, opacity, monochrome, grayscale, invert) | Yes | — | "Filters" label on Windows |
| Color Filters (grayscale, sepia, invert, sharpen, opacity) | — | Yes | "Color Filters" label on Mac |
| Color Replacement | Yes | No | Swap specific colors |
| Capture Info | Yes | Yes | Stamps app/version/date/OS metadata |

> *(uncertain)* The exact filter sub-options per platform shift across releases; treat the parenthetical
> lists as representative rather than exhaustive.

---

## 2. Structural / framing effects

### 2.1 Border
Adds a solid outline around the whole image (or selection on Windows).
- **Options:** color (preset swatch, palette, eyedropper, or hex/RGB), width slider.
- **Flow:** Effects > Border > pick color + width > Apply (Win) / toggle + gear (Mac).

### 2.2 Edges (edge effects)
Applies a stylized edge to one or more sides of the image.
- **Styles:** drop shadow, **torn**, **faded**, **wave**, **saw**, **shark tooth**, and **beveled**.
- **Options:** choose which side(s) get the effect, edge **size**, and (Windows) **shadow color and
  location**.
- **Flow:** Effects > Edges > select style > choose sides/size > Apply.

### 2.3 Shadow
Drop shadow on selected sides or all around the image.
- **Windows:** grid selector for direction + size.
- **Mac:** adds **opacity**, **offset**, and **blur** sliders.

### 2.4 Perspective
Distorts the image to imply a viewing angle/tilt.
- **Options:** horizontal/rotation slider and vertical/tilt slider.

### 2.5 Page Curl *(Windows only)*
Curls a corner of the image for a 3D "peeling page" look. Simple preset + Apply.

### 2.6 Reflection *(Mac only)*
Adds a mirror-like reflection beneath the image. Options: reflection **size %** and **opacity**.

### 2.7 Fade *(Mac only)*
Gradually fades/dims the image edges with a smooth gradient.

### 2.8 Capture Info
Overlays capture metadata: application name, version, date/time, OS, plus a custom text field.
- **Options:** background color, font properties. Available on Windows and Mac.

---

## 3. Color / tone effects

### 3.1 Color Adjustment
Per-channel tonal controls (both platforms):
- **Brightness** — overall lightness
- **Contrast** — difference between light/dark
- **Hue** — shifts dominant color
- **Saturation** — color intensity
- **Gamma** — overall darkness/midtone curve

### 3.2 Filters (Windows) / Color Filters (Mac)
- **Windows "Filters":** soften, sharpen, opacity, **monochrome**, **grayscale**, **invert colors**.
- **Mac "Color Filters":** **grayscale**, **sepia**, **invert**, **sharpen**, **opacity**.
- *(uncertain)* A dedicated standalone "Sepia" preset is documented on Mac; on Windows sepia‑style
  results are typically achieved via monochrome/color adjustment rather than a named sepia toggle.

### 3.3 Color Replacement *(Windows only)*
Swaps one or more specific colors for others. Add **color pairs** (source → replacement) before
applying. Useful for recoloring UI elements or brand color swaps.

---

## 4. Focus effects vs focus tools (Spotlight / Magnify)

Snagit offers two overlapping ways to draw attention to an area, and which one you get depends on
the OS. This distinction matters for a clone.

### 4.1 Spotlight and Magnify — **effect** *(Windows)*
Highlights and enlarges a selected region in one combined operation.
- **Flow:** select an area with the **Selection** tool > `Image > Effects > Spotlight and Magnify` >
  adjust properties > **Apply**.
- Dims/blurs the surrounding background while magnifying the selected region.

### 4.2 Spotlight — **tool** *(Mac only)*
Calls attention to an area by **dimming and/or blurring the rest** of the image (no magnification).
- **Shapes:** rectangle, rounded rectangle, oval, polygon (click points, double‑click to close).
- **Options:** surrounding **background color**, **opacity** slider, **blur** slider, and an
  **"Apply only to text"** option.
- Windows users approximate this with the Selection tool + Spotlight & Magnify effect.

### 4.3 Magnify — **tool** *(Windows + Mac)*
A toolbar annotation tool (object), distinct from the Windows effect. Enlarges part of the image to
show detail; the magnified region stays an editable object.
- **Flow:** select Magnify tool > pick a Quick Style or customize > click‑drag on canvas to define
  the area > resize via white handles > drag the anchor to reposition the lens > switch to Move tool.
- **Options:** shape (**circle / square / rectangle**), **magnification %** (slider or value),
  outline **color** and line **width**.
- Newer builds (Windows 2026.1.0+, Mac 2025.3.0+) support selecting **all** magnifications at once
  for batch edits.

> **Clone takeaway:** treat Magnify as a live *object* (annotation) and Spotlight/Spotlight&Magnify as
> a *baked effect/region operation*. The Windows "effect" and Mac "tool" are functionally similar but
> architecturally different.

---

## 5. Watermark *(primarily Windows)*

Overlays text, a logo, or copyright info onto the image.
- **Source:** import an image file (logo) or use text.
- **Options:** **position grid**, **scale** slider, and display mode:
  - **Underlay** — embossed into the image background.
  - **Overlay** — placed on top of the image.
- **Flow:** Effects > Watermark > choose image/text > set position + scale > Underlay/Overlay >
  Apply.
- **Mac workaround:** Mac lacks a dedicated Watermark effect; users drag a logo onto the canvas
  (optionally with background removed) and lower its **opacity**, or use a **Stamp**.

**Related — Stamps:** Snagit's Stamp tool can add a saved logo/graphic. Add via Stamp tool >
Properties > Organize Stamps > Add Stamp > select image. Stamps are reusable annotation graphics
(not strictly a "watermark" but the practical Mac path to a logo overlay).

---

## 6. Canvas & image transforms

### 6.1 Crop
Removes the outer edges of the image.
- **Flow:** Crop tool in toolbar > drag **blue handles** to set region > drag selection to
  reposition > click **Crop** to delete everything outside the region.
- **Circle/round crop:** the **Selection** tool can produce a circular/rounded image; dragging a
  **yellow diamond** handle toward the center rounds corners (good for circular avatars).

### 6.2 Trim / Auto Trim (auto-remove whitespace)
- TechSmith support refers to an **Auto Trim** capability that removes unwanted padding/whitespace
  from image edges automatically (detecting a uniform border).
- *(uncertain)* The current Editor's exact entry point for Trim was not confirmed on a live tutorial
  page in this pass. Historically it is invoked from the **Image** menu (e.g., `Image > Trim`).
  Practical alternatives documented today: the **Fill** tool (replace a solid border color with
  transparent) and **Crop** for manual edge removal. For a clone, implement Trim as: scan inward
  from each edge, stop where pixel color deviates from the corner color beyond a tolerance, then crop
  to that bounding box.

### 6.3 Cut Out
Deletes a full-width **horizontal** or full-height **vertical** strip and stitches the remaining
parts together — ideal for removing big empty gaps in long screenshots while keeping both ends.
- **Flow:** Cut Out tool > choose a **Quick Style** (sets direction + edge treatment) > click‑drag
  across the image to define the strip to remove > switch tools to deactivate.
- **Edge styles:**
  - **Straight edges:** sections are stitched **seamlessly** (no visible seam).
  - **Other (decorative) edges:** Snagit leaves a small **gap** with a **torn/cut line** between the
    rejoined sections, visually signaling that content was removed.
- **Orientation:** Horizontal and Vertical on both platforms.
- **Mac extras:** **Add Horizontal Space** and **Add Vertical Space** styles *expand* the image by a
  specified pixel amount (the inverse of cutting). Re-applying Horizontal/Vertical within an existing
  gap reduces that gap.

### 6.4 Resize Image
Changes the actual pixel dimensions of the image.
- **Entry:** `Image > Resize Image`, or click the dimensions readout at the bottom of the canvas.
- **Units:** **percent**, **pixels**, **inches**, **centimeters**.
- **Aspect ratio:** lock icon to keep proportions.
- **Windows extras:** **Resolution** (DPI) and **Resampling** — adding/removing pixels to change
  file size; enabling resampling **can permanently degrade quality**.
- **Mac extras:** set **pixel density** (pixels per inch) before resizing.

### 6.5 Resize Canvas / Expand Canvas
- Resizing the **canvas** (vs. the image) changes the working area without scaling pixels — used to
  add space around an image or to crop by shrinking.
- *(uncertain)* In the current Editor, expanding the canvas is generally done by **dragging the
  canvas edge** outward (which adds canvas‑colored space); older Snagit versions exposed an explicit
  **Canvas Size** dialog. Confirm the exact UI for the target version.
- When you alter a capture by resizing the canvas, Snagit preserves the original as a **`.snagundo`**
  file and writes a new `.snag` file (non-destructive history).

### 6.6 Rotate
- **Presets:** 90° clockwise, 90° counter‑clockwise, 180°.
- **Custom angle:** enter any positive integer degree value.
- **Mac:** also supports **click‑drag interactive rotation** via a control circle.
- **Windows:** rotate button handles 90° increments (plus the custom-degree field).

### 6.7 Flip
- **Flip Horizontal** (left↔right) and **Flip Vertical** (top↔bottom).
- Flipping also flips **text and annotations** within the image (they are mirrored, not just the
  background).

### 6.8 Canvas Color
- `Image > Canvas Color` (Windows) / `Image > Change Canvas Color` (Mac).
- Click the canvas color preview, pick a color or the **transparent** option (checkerboard pattern),
  then OK/Apply. Transparent canvas exports with alpha (PNG).

---

## 7. Simplify tool (Simplified User Interface / SUI)

The **Simplify** tool replaces text and detailed UI elements with clean, abstract shapes so a
screenshot stays generic — easier to localize, version‑proof, and less visually noisy. This is one of
Snagit's signature differentiators.

### 7.1 Auto Simplify (auto-detect)
- Enable **Auto Simplify** in Tool Properties; Snagit scans the image and **overlays detected text
  and UI elements with simplified objects** (colored bars/blocks/shapes approximating buttons,
  fields, text runs, etc.).
- **Detail slider:** adjusts detection **sensitivity**; dragging it **regenerates** the set of
  objects. Higher detail = more, smaller objects; lower = fewer, coarser blocks.

### 7.2 Color Palette
- The **Color Palette** panel controls the colors used for simplified objects.
- Click a color dropdown to recolor objects on the canvas; repeat to define the palette.
- **Save palettes:** settings/gear icon > **Save Palette** > create new or replace existing — for
  brand/product consistency across many simplified images.

### 7.3 Manual simplify objects
You can add and edit simplified shapes by hand:
- **Quick Styles** panel: preset shapes to drop onto the canvas.
- **Draw Mode** (add new objects) vs **Move Mode** (reposition existing ones).
- **Per-object editing:** drag handles to resize, click‑drag to reposition, **Delete** to remove,
  and edit **fill color, shape type, background color, text height, line spacing**.
- **Select All (Shape):** batch‑edit all simplify shapes at once.

### 7.4 Show Original
- **Show Original** reveals the underlying real screenshot beneath the simplified overlay, with an
  **opacity slider** so you can line up shapes against the source while editing.

### 7.5 Integration with annotations
- Simplify objects coexist with normal annotations (arrows, callouts, etc.) on the same canvas; you
  switch tools freely. Simplified shapes remain editable objects until flattened/exported.

### 7.6 Platform note
- **Windows** includes a **Lock Objects** option that prevents accidental edits to simplify objects
  when switching tools; **Mac** does not expose this option.

> **Clone takeaway:** Auto Simplify needs (a) a layout/UI element detector to produce bounding boxes,
> (b) a mapping from box → simplified shape primitive, (c) a managed color palette, and (d) editable
> shape objects with a Show‑Original underlay. The Detail slider maps to detector granularity/merge
> threshold.

---

## 8. Move tool & Smart Move (auto-detect to rearrange)

### 8.1 Move tool
Selects, moves, edits, or deletes annotation **objects** on the canvas. After drawing with other
tools, switch to Move to drag/resize/delete them or double-click to edit properties.

### 8.2 Smart Move (auto-detect objects in a flat screenshot)
Smart Move makes elements of a **flattened screenshot** editable as if they were objects.
- **What it does:** "automatically detects UI elements in your screenshots, allowing you to
  rearrange or remove them easily." Hover an editable object and selection handles appear.
- **Enable:**
  - **Windows:** turn on **Smart Move** in the **Tool Properties** panel (of the Move tool).
  - **Mac:** click the **Smart Move** button in the **Actions** panel.
- **Detail slider:** controls detection granularity. Slide **right** to detect smaller objects/lines/
  text and group fewer things together; **left** to group more together.
- **Actions per object:**
  - **Move** — click‑drag to a new position.
  - **Delete** — press Delete.
  - **Edit Text** — double‑click a text box to edit its text in place.
- **Auto-fill background:** when an object is moved or deleted, Snagit auto-fills the vacated area to
  match the surrounding background (same auto-fill engine as the Selection tool).
- **Privacy / tech:** runs **locally** via computer-vision (TechSmith cites **OpenCV**). Neither the
  image nor detection data is uploaded; nothing is used for training.

> **Distinction:** **Smart Move** = make a *static* screenshot's UI editable to rearrange/remove.
> **Simplify** = replace UI with *abstract shapes*. They share the same "auto-detect UI elements"
> heritage but produce different outputs.

---

## 9. Grab Text & Edit Text (OCR)

Snagit uses OCR to pull text out of images. (OCR engine historically **ABBYY**; license renewed via
a May 1, 2026 update — see top caveat.)

### 9.1 Grab Text from a capture (during capture)
- In the **Capture** window, select the **Image** tab, choose **Grab Text** from the **Selection**
  dropdown.
- Click **Capture** (or **Print Screen** on Windows / **Shift+Ctrl+C** on Mac), then drag to select
  the on‑screen area.
- The **Grab Text Results** dialog opens in the Editor with the recognized text.

### 9.2 Grab Text from an existing image (in the Editor)
- **All text:** right‑click (Windows) / control‑click (Mac) the image on the canvas > **Grab Text**.
- **Partial text (more accurate):** use the **Selection** tool to box a smaller text region first,
  then right‑click > **Grab Text**. Limiting the area improves recognition accuracy.

### 9.3 Grab Text Results dialog
- **Copy All:** copies the recognized text to the clipboard for pasting elsewhere.
- **Plain text vs formatted:** enable **Plain text** (Windows) / **Make Plain Text** (Mac) to strip
  font and color, then **Copy All**. The plain‑vs‑formatted choice in the Results dialog was added in
  **Snagit 2024**.
- *(uncertain)* Whether Snagit can auto-copy Grab Text output to the clipboard without a click is not
  a built‑in toggle in older versions (community requests exist); confirm for the target version.

### 9.4 Edit Text (make screenshot text editable in place)
Rather than just copying, Snagit can convert recognized text into an **editable text object** on the
image:
- **Flow:** Selection tool > box the text region > right‑click > **Edit Text**.
- Then via **Tool Properties** you can change **font, size, color**, insert new text, **replace**
  selected words, or delete characters — directly on the screenshot. Useful for updating labels/values
  in a screenshot without re-capturing.

### 9.5 Languages & accuracy
- Supported languages (per support docs): **English, German, French, Japanese, Spanish** — with the
  caveat that **Japanese characters** are noted as a limitation.
- **Accuracy best practices:**
  - High **contrast** between text and background is critical; light‑on‑light recognizes poorly.
  - Select a **smaller region** rather than the whole image to boost accuracy.
- Related OCR-powered features in the same family: **Smart Redact** (auto-detect and redact sensitive
  text) and **Translate Text**.

### 9.6 Platform notes
- Core Grab Text/Edit Text works on both Windows and Mac.
- **Mac** can grab text from a **paused video frame**.
- Capture-time shortcut differs: **Print Screen** (Win) vs **Shift+Ctrl+C** (Mac).

---

## 10. Editions (which tier has the AI/OCR features)

TechSmith currently markets three tiers: **Snagit**, **Snagit Create**, **Snagit Pro**.
- **Snagit (base):** core capture/recording, scrolling capture, Library, and AI features including
  **Smart Move** and **Text Recognition** (Grab Text).
- **Snagit Create:** base + 100M+ premium images, templates, and AI visuals.
- **Snagit Pro:** Create + unlimited Screencast sharing (recommended tier).
- *(uncertain)* The support article does **not** publish a full per-tier matrix for every AI feature
  (Simplify, Edit Text, Smart Redact). Base Snagit is confirmed to include Smart Move and text
  recognition; assume Simplify is in base as well (long-standing core feature) but verify for the
  target version. Create/Pro are currently individual-only (online).

---

## 11. Clone implementation checklist (derived)

- **Effects engine:** whole-image, non-destructive, re-applyable "Quick Effects"; Windows-style
  selection-scoped application optional. Effects list: Border, Edges (7 styles), Shadow, Perspective,
  Page Curl, Reflection, Fade, Color Adjustment (5 sliders), Filters (grayscale/sepia/invert/
  sharpen/soften/monochrome/opacity), Color Replacement (color pairs), Watermark (overlay/underlay),
  Capture Info, Spotlight & Magnify.
- **Transforms:** Crop (handles), Trim/Auto-Trim (corner-color flood bounding box), Cut Out (strip
  removal + straight stitch vs decorative gap/torn line, H/V), Resize Image (units + aspect + DPI +
  resample), Resize/Expand Canvas (drag edges, undo history), Rotate (90/180/custom/drag), Flip
  (H/V, also mirrors annotations), Canvas Color (incl. transparent).
- **Simplify:** detector + bounding boxes → shape primitives, Detail/granularity slider, managed
  saveable color palettes, manual Draw/Move modes, per-object props, Show Original underlay, Lock.
- **Smart Move:** local CV detection (OpenCV-style), Detail granularity, move/delete/edit-text,
  auto-fill on vacate.
- **OCR:** Grab Text (region or whole image, capture-time and in-editor), Results dialog with
  Copy All + plain/formatted toggle, Edit Text in-place, language packs, contrast-driven accuracy.

---

## Sources

- Add Effects — Snagit Tutorial: https://www.techsmith.com/learn/tutorials/snagit/effects/
- Simplify Images — Snagit Tutorial: https://www.techsmith.com/learn/tutorials/snagit/simplify/
- Simplified User Interface: The Beginner's Guide — TechSmith Blog: https://www.techsmith.com/blog/simplified-user-interface/
- Grab & Edit Text — Snagit Tutorial: https://www.techsmith.com/learn/tutorials/snagit/edit-image-text/
- Snagit Grab Text & Edit Text Best Practices — TechSmith Support: https://support.techsmith.com/hc/en-us/articles/115002031592-Snagit-Grab-Text-Edit-Text-Best-Practices
- Issues With Grab Text or Other OCR Functions (May 2026 OCR license note) — TechSmith Support: https://support.techsmith.com/hc/en-us/articles/45409168722445-Issues-With-Grab-Text-or-Other-OCR-Functions-Such-as-Edit-Text-or-Smart-Redact
- How to Extract Text from an Image — TechSmith Blog: https://www.techsmith.com/blog/extract-text-from-image/
- Translate Text — TechSmith Support: https://support.techsmith.com/hc/en-us/articles/360050660631-Translate-Text
- Crop Tool — Snagit Tutorial: https://www.techsmith.com/learn/tutorials/snagit/crop/
- Cut Out Tool — Snagit Tutorial: https://www.techsmith.com/learn/tutorials/snagit/cut-out/
- Remove Parts of an Image — Snagit Tutorial: https://www.techsmith.com/learn/tutorials/snagit/remove-image-parts/
- Resize, Rotate, or Flip an Image — Snagit Tutorial: https://www.techsmith.com/learn/tutorials/snagit/resize-rotate-flip-image/
- Move Tool — Snagit Tutorial: https://www.techsmith.com/learn/tutorials/snagit/move/
- What is Smart Move? — TechSmith Support: https://support.techsmith.com/hc/en-us/articles/39980449934093-What-is-Smart-Move
- Magnify Tool — Snagit Tutorial: https://www.techsmith.com/learn/tutorials/snagit/magnify/
- Spotlight Tool (Mac) — Snagit Tutorial: https://www.techsmith.com/learn/tutorials/snagit/spotlight/
- Spotlight and Magnify (webinar) — TechSmith: https://www.techsmith.com/learn/webinars/spotlight-and-magnify/
- How to Add a Watermark to Photos — TechSmith Blog: https://www.techsmith.com/blog/watermark-images/
- Crop Image (feature page) — TechSmith: https://www.techsmith.com/snagit/features/crop-image/
- Magnify Image (feature page) — TechSmith: https://www.techsmith.com/snagit/features/magnify-image/
- What Is the Difference Between Snagit Pro, Snagit Create and Snagit? — TechSmith Support: https://support.techsmith.com/hc/en-us/articles/41688272400781-What-Is-the-Difference-Between-Snagit-Pro-Snagit-Create-and-Snagit
- Resize or Remove White Space in PDF in Snagit — TechSmith Support: https://support.techsmith.com/hc/en-us/articles/203732308-Resize-or-Remove-White-Space-in-PDF-in-Snagit
- Snagit Windows 2026 Version History — TechSmith Support: https://support.techsmith.com/hc/en-us/articles/42674936732685-Snagit-Windows-2026-Version-History

*Secondary/illustrative (not authoritative): groovypost.com Snagit reviews, keystrokelearning.com.au Simplify guide, bforbloggers.com Snagit review.*
