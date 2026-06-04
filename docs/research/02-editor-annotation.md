# Snagit Editor — Canvas & Annotation Tools (Research)

> Research compiled from official TechSmith sources (support.techsmith.com, techsmith.com/learn/tutorials/snagit) plus the latest version history pages. Snagit ships on **Windows** and **macOS** as two separate codebases; behavior is largely parallel but several features are platform-specific. These are flagged inline. Where the official docs were ambiguous, this is noted as an **Uncertainty**.
>
> Scope: this document covers the **Snagit Editor** (the post-capture editing surface) and its **annotation / markup / editing tools**. It does **not** cover the Capture window, video recording, or Camtasia hand-off (annotation tools and Effects are image-only; video must be sent to Camtasia, with live drawing handled by **Screen Draw** during recording).

---

## 1. Snagit Editor — Overall Layout

The Snagit Editor is where you review, edit, annotate, enhance, and share captures. It is also the hub for accessing capture history (Library) and TechSmith Assets (stamps, themes, templates).

### 1.1 Named UI regions

| Region | Location | Purpose |
|---|---|---|
| **Editor / Library tabs** | Top-left | Switch between the **Editor** (edit a single capture) and the **Library** (browse capture history). |
| **Toolbar** | Top (horizontal) | Holds the annotation/editing tools (Arrow, Text, Callout, Shape, Step, etc.). Click **More** (`…`) to reveal tools that don't fit / are hidden. Customizable. |
| **Tool Properties panel** | Right side | Shows the properties for the currently selected tool (color, font size, width, shape, opacity, shadow, etc.). Contents change per tool. |
| **Quick Styles** | Right side, within/above the Properties panel | Row of preset styles for the active tool; also where you save custom styles and add to Favorites. |
| **Effects** | Right-side panel / Effects button | Image-level effects (Border, Edges, Color Adjustment, etc.) and **Quick Effects** presets. Separate from per-object tool properties. |
| **Canvas (Workspace)** | Center | The editing surface that displays the selected capture. Drag the handles on the canvas edges to expand the working area (adds blank canvas around the image). |
| **Recent Captures Tray** | Bottom (horizontal strip) | Thumbnails of recent image/video captures and other files opened/edited in Editor. The selected thumbnail is shown on the canvas. |
| **Library button / tab** | Top-left | Opens the full Library — capture history (images, videos, GIFs) searchable by date, source, tags, etc. |
| **Share toolbar / options** | Top-right | Sharing destinations: **Copy All**, **Share Link**, plus a dropdown to send to apps, cloud, document, social media. |
| **Menu bar** | Top (Windows: in-app menus; Mac: macOS menu bar) | File / Edit / Image / View / etc. Houses commands like Canvas Snapping, Flatten All, Grab Text, undo/redo. |
| **Assets panel** | Accessible from Editor | Access to TechSmith Assets — 100M+ stamps, photos, themes, templates. |

### 1.2 Customizing the toolbar

- **More menu → Customize Toolbar**: drag tools to/from the toolbar to show the ones you use and hide the rest. Hidden tools remain available under **More**.
- Tool layout, Quick Styles, and Favorites are user-configurable.

### 1.3 Image vs. Video

- All annotation tools, the Move tool, and Effects are **image-only**.
- For video, share to **Camtasia** for annotations/effects, or use **Screen Draw** to draw live during a recording.

---

## 2. Vector vs. Raster (editability model)

This is the conceptual backbone of how annotations behave.

- **Vector objects stay editable.** Annotations you add — Text, Callouts, Arrows, Shapes, Lines, Stamps, Steps, pasted images, etc. — are placed as separate **vector objects** on top of the image. They can be re-selected, moved, resized, restyled, reordered, or deleted at any time using the **Move** tool.
- **Editable file format:** Save as **.SNAG / .SNAGX** to preserve all vector objects and the current editable state. Reopening the `.snag/.snagx` file lets you continue editing every object.
- **Flattening = becomes raster.** When you save/export as **PNG or JPEG** (or use **Flatten All**), all objects are merged ("flattened") into the bitmap and are **no longer editable** as separate objects.
- **Layering / z-order:** Each object sits on its own layer; newer objects are drawn on top. Objects can be reordered (send to back / bring to front via arrange controls and right-click context menu) — see §4.18.
- **Raster-only tools** operate directly on the flattened pixels rather than creating vector objects: **Fill (bucket)**, **Eraser**, **Blur**, **Crop**, **Cut Out**, and **Selection**-based pixel edits. Per the docs, Fill and Eraser act on "flattened" areas of the image.
- **Pen** and **Line** can be either vector or raster depending on the **Create as Vector** (Windows) / **Create Editable Line** (Mac) toggle.

> **Cross-platform caveat:** Effects applied in **Snagit Mac** are not currently supported by **Snagit Windows** when saved as a **SNAGX** file — they will not display and are lost on Windows. Saving as PNG/JPG preserves the rendered result across platforms.

---

## 3. Quick Styles, Favorites, Themes & Colors

### 3.1 Quick Styles (tool presets)

- Every tool ships with default **Quick Styles** — preset combinations of that tool's properties for one-click annotation.
- **Save a custom Quick Style:** select a tool → adjust properties in the Properties panel → a new style preview appears below the existing styles → click the **+ / save icon** to keep it.
- **Reorder:** drag styles to reposition.
- **Delete:** right-click a style → **Delete Quick Style**.

### 3.2 Favorites

- Click the **star** on any Quick Style or Quick Effect to add it to **Favorites** (a consolidated, cross-tool collection).
- The star turns **yellow** when saved; click again to remove.

### 3.3 Quick Effects

- Preset, customizable **effects** (image-level). Windows: save a custom effect via **Add to Quick Effects**. Mac: a new effect preview appears automatically with a save icon.

### 3.4 Themes (brand/theme colors)

- A **Theme** is a coordinated **color palette (recommended 4–8 colors) + a single font (+ optional drop shadow on/off)** used to keep annotations visually consistent across a project/site/organization.
- When you create a theme, **Snagit auto-generates a set of Quick Styles** from the theme colors, which then appear across most annotation tools.
- **Create:** choose colors (presets, palette, HEX/RGB, or eyedropper) → pick a font → name it → **Create**.
- **Edit / Export:** via the **gear icon**; export as a **.SNAGTHEME** file to share with teammates. Import shared theme files, or download professionally designed themes from **Snagit Assets** (requires Windows 13.1+ / Mac 4.1+).
- This is Snagit's mechanism for **brand colors** — define brand colors once, propagate as Quick Styles everywhere.

### 3.5 The color control (shared across tools)

Most tools' color dropdowns offer:
- Preset **theme colors**,
- **HEX / RGB** entry,
- an **Eyedropper** to sample a color from anywhere on screen,
- (Mac) **gradient** options on some tools (e.g., Callout fill).

---

## 4. Annotation & Editing Tools

> **Universal UX flow** for most annotation tools: **select tool in toolbar → Quick Styles + Tool Properties appear on the right → pick a Quick Style or customize properties → click (or click-drag) on the canvas to create the object → type/adjust as needed → switch to the Move tool to re-select, reposition, restyle, or delete.** This pattern is repeated below and abbreviated where it's identical.

### 4.1 Arrow

- **Purpose:** Draw arrows to direct attention.
- **Create:** click-drag from **tail → head**. Hold **Shift** to constrain to horizontal/vertical (or 45° increments).
- **Properties:**
  - **Color** (theme/HEX/RGB/eyedropper)
  - **End Style** — arrowhead style at the end (various head shapes)
  - **Line Style** — solid / dashed / dotted
  - **Width** — line thickness in pixels (slider)
  - **Start/End size** — arrowhead proportion (slider)
  - **Opacity** — transparency slider
  - **Shadow** — direction + advanced (color, distance, opacity, blur; angle on Windows)
  - **Bezier curve** — enable curved arrows; drag the white handles to shape the curve (available on Windows and Mac)
- **Re-edit:** Move tool → drag endpoints, restyle, or delete.

### 4.2 Text

- **Purpose:** Add captions, headings, labels directly as text (no background box).
- **Create:** click-drag to make a text box → type.
- **Properties:**
  - **Font**, **Font size** (slider/entry)
  - **Bold / Italic / Underline / Strikethrough** (availability varies by platform)
  - **Fill color** (letter interior) and **Outline color** + outline **line width** (set to 0 for no outline)
  - **Alignment** (horizontal/vertical)
  - **Opacity**
  - **Shadow** (direction + color, distance, opacity, blur; angle on Windows)
  - **Padding** (Windows only)
  - **Select All Text Objects** — bulk-edit all text objects at once (newer versions)
- **Note:** For a background box/border or a pointing tail, use the **Callout** tool instead.

### 4.3 Callout (speech bubbles)

- **Purpose:** Annotate with a text box that has a shape, fill, and a pointing **tail**.
- **Shape styles:** multiple via the **Shape Style** dropdown (rounded rectangles, speech/thought bubbles, ovals, etc.).
- **Create:** click-drag to draw the callout → type text.
- **Handles:** white = resize; **yellow** = move the tail tip; **blue** = add an additional tail; **green** = rotate; an **anchor point** repositions the callout body relative to the tail.
- **Properties:**
  - **Fill color** (theme/HEX/RGB/eyedropper; Mac adds gradient)
  - **Outline color**
  - **Shadow** (direction + advanced color/distance/opacity/blur)
  - **Arrow/Tail style** — line style + end style + size slider + color
  - **Text:** font, font color, size, **B/I/U/strikethrough**, alignment (H/V), **padding** + **opacity** (Windows)
- **Re-edit:** Move tool → reshape, retype, drag tail, rotate.

### 4.4 Shape

- **Purpose:** Add a shape / frame an area.
- **Create:** click-drag a **rectangle** or **ellipse**; choose **Polygon** in properties (drag first side, then click to anchor each corner). Hold **Shift** for a perfect square/circle.
- **Properties:**
  - **Fill color** — incl. **transparent** for a frame-only shape
  - **Outline color**
  - **Line width** (px) and **Line style**
  - **Opacity**
  - **Shadow** (direction + color, angle, distance, blur, opacity)
- **Re-edit:** Move tool → white handles resize, green handle rotates.

### 4.5 Line

- **Purpose:** Add a straight or curved line (no required arrowhead, but heads optional).
- **Create:** click-drag. Hold **Shift** for horizontal/vertical.
- **Properties:**
  - **End style** (optional arrowhead)
  - **Line style** (solid/dashed)
  - **Color**, **Width** (px), **Opacity**
  - **Shadow** (direction + color, angle, distance, blur)
  - **Bezier curve** checkbox → drag white handles to curve
- **Re-edit:** Move tool → white handles resize/move.

### 4.6 Highlighter (Highlight)

- **Purpose:** Semi-transparent highlight over a region while keeping underlying text/objects visible (like a marker).
- **Create:** click-drag the area to highlight.
- **Properties:**
  - **Color**, **Opacity** (semi-transparent)
  - **Shape** (Mac only): rectangle, rounded rectangle, oval, polygon (double-click to finish polygon)
  - **Apply only to text** (Mac only) — highlight just the text, not the whole region
- **Distinction:** This is the **Highlighter annotation tool** (creates a translucent highlight object). It differs from the **Spotlight & Magnify effect** (which dims/emphasizes). On Windows, more shaped emphasis is done via the Selection + Spotlight/Magnify effect.

### 4.7 Step (numbered/lettered stamps / sequence)

- **Purpose:** Click to drop auto-incrementing numbered or lettered badges to illustrate a sequence/process.
- **Create:** click the canvas to drop step 1; each subsequent click increments automatically.
- **Properties:**
  - **Type** — sequence format: numbers (1,2,3…), letters (A,B,C… / a,b,c…), **Roman numerals**
  - **Fill color**, **Text color**, **Shape** (circle, square, etc.)
  - **Shadow** (direction, angle, distance, opacity, blur), **Opacity**
  - **Restart Sequence** — reset the count for the next steps
  - **Select All** — edit all step objects at once (newer versions)
- **Manual edit:** right-click a step → **Edit Value** to change its number/letter.
- **Re-edit:** Move tool to reposition; the sequence renumbers logically.

### 4.8 Stamp (stickers / graphics)

- **Purpose:** Drop graphic stamps/stickers (cursors, emoji, icons, badges, etc.).
- **Create:** choose a **category** from the dropdown or **search** → click the canvas to place → drag to move, handles to resize.
- **Custom stamps:** import **JPG / PNG / GIF** as stamps. **Mac** can also save a Snagit-created/captured image as a custom stamp.
- **Assets / downloads:** hundreds of free stamps from TechSmith **Assets**. Windows: **Download More** in Quick Styles. Mac: browse/import stamp sets.
- **Properties:** **Shadow** (direction, color, angle, distance, opacity, blur), **Opacity**. Save as Quick Styles.
- **Re-edit:** Move tool.

### 4.9 Pen (Freehand)

- **Purpose:** Freehand drawing.
- **Create:** click-drag to draw. Hold **Shift** for a straight horizontal/vertical line.
- **Vector vs. raster:** **Create as Vector** (Windows) / **Create Editable Line** (Mac) — when enabled, the stroke is an editable object; when disabled, it's painted permanently into the bitmap.
- **Properties:**
  - **Color**, **Width** (px), **Opacity**
  - **Shape** (Windows: multiple; Mac: circle only)
  - **Shadow** (direction, color, distance, opacity, blur)
  - **Antialias** + **smooth curve** smoothing options
  - **Select All** — bulk edit pen objects
- **Re-edit:** Move tool (only for vector/editable strokes).

### 4.10 Fill (bucket)

- **Purpose:** Replace/flood a contiguous color region — hide objects by filling same-colored pixels (raster operation).
- **Create:** pick a fill color (use the **Eyedropper** to sample) → click the area to fill.
- **Properties:**
  - **Color** (incl. eyedropper)
  - **Tolerance** slider — how much same-color area is filled (higher = broader, lower = precise)
- **Note:** Operates on flattened pixels; not an editable vector object.

### 4.11 Eraser

- **Purpose:** Erase flattened areas of the image to expose the underlying canvas.
- **Create:** drag over pixels to erase.
- **Note:** Raster operation (acts on flattened image data). The official tool page gave limited property detail — **Uncertainty:** exact brush-size/shape options for the Eraser were not confirmed in the fetched docs.

### 4.12 Selection

- **Purpose:** Select part of the image to **cut, copy, move, delete, or apply effects** to that region (raster selection).
- **Shapes:** **Rectangle**, **Ellipse**, **Polygon** (click corners, double-click to finish), **Freehand/Lasso** (drag).
  - Hold **Shift** for perfect square/circle.
- **Actions on a selection:**
  - **Move** (drag), **Delete** (Delete key), **Copy/Paste** (Ctrl/Cmd+C/V)
  - **Grab Text** via right-click (Win) / control-click (Mac)
  - **Effects** button for quick/custom effects on the selection
  - **Invert** — select everything *except* the region
- **Snap to Object:** auto-snaps the selection to the nearest edges of a shape/icon inside the selected area.
- **Background fill when moving/deleting:**
  - **Transparent** — reveals canvas underneath
  - **Auto-Fill** — intelligently fills the vacated area based on surroundings

### 4.13 Move (and Smart Move / Grab Text move)

- **Purpose (basic):** Select, move, resize, rotate, restyle, or delete the **vector objects** you've added.
  - Double-click an object to edit it (text/properties); Delete key removes it.
  - This is the tool you switch to after creating any annotation.
- **Smart Move:** AI-assisted feature that **automatically detects objects inside the screenshot itself** (buttons, text, UI elements) and makes them editable in place — rearrange buttons, delete/replace text, move UI pieces.
  - Runs **locally** using **OpenCV** (no upload of your image).
  - Hover shows selection handles over detected objects; a **Detail** slider tunes detection granularity.
  - Double-click detected text to edit it.
  - **Add Narration** (Mac) — record audio commentary for image sequences/slideshows.
- **Re-editing model:** the Move tool is the gateway to all post-creation editing of vector objects, including layering/arrange (see §4.18).

### 4.14 Cut Out

- **Purpose:** Delete a **horizontal or vertical band** out of an image and join the remaining pieces (e.g., remove whitespace/long gaps).
- **Create:** choose a Quick Style (sets **direction** + **edge style**) → click-drag across the band to remove.
- **Edge behavior:** **straight-edge** styles → Snagit **seamlessly stitches** the two sides together; **decorative-edge** styles → Snagit leaves a (non-adjustable) **gap** with a styled edge.

### 4.15 Crop

- **Purpose:** Trim unwanted areas from the **edges** of the image.
- **Create:** drag the **blue handles** to define the keep-area → reposition by dragging → click the **Crop** button to apply.
- **Note:** Crop changes the canvas/image dimensions (destructive to trimmed pixels once applied).

### 4.16 Magnify

- **Purpose:** Enlarge a region in place to show detail / draw attention (a magnified inset).
- **Create:** click-drag to select the area → it renders magnified; reposition the magnified inset via an **anchor point**; white handles resize.
- **Properties:**
  - **Magnification %** (slider or value entry)
  - **Outline color**, **Line width** (px)
  - **Shape** options
  - **Select All (Magnification)** — edit all magnify objects at once (newer versions)
- **Re-edit:** Move tool.

### 4.17 Blur (and Pixelate / Redact)

- **Purpose:** Obscure sensitive/distracting info.
- **Types (in properties):**
  - **Blur** — soft, out-of-focus-lens smoothing
  - **Pixelate** — large square pixels (good for faces/IDs)
  - **Solid shape** — fully opaque cover; **recommended for true redaction** because it can't be reversed by third-party tools
- **Intensity** slider controls strength.
- **Shape** options via dropdown.
- **Create:** click-drag the area to obscure.
- **Smart Redact:** AI feature that auto-detects and covers sensitive data (emails, phone numbers, credit-card numbers, etc.) across categories.
- **Security note:** plain Blur/Pixelate may be partially reversible — use **solid shape** for sensitive redaction.

### 4.18 Spotlight (Mac) / Spotlight & Magnify (Windows effect)

- **Purpose:** Emphasize a region by **dimming and/or blurring the surrounding** area.
- **Mac (Spotlight tool):**
  - **Shapes:** rectangle, rounded rectangle, oval, polygon
  - **Properties:** **Color** (surround background color), **Opacity** (of the dimming background), **Blur** (clarity of the surrounding background), **Apply only to text**
  - **Create:** click-drag to set the spotlight region; white handles resize; green handle rotates.
- **Windows:** achieved via the **Selection tool + the Spotlight & Magnify effect** under **Effects** (combines highlighting + enlargement of a selected area).

### 4.19 Simplify (SUI — Simplified User Interface)

- **Purpose:** Replace text and busy UI details with clean, **simplified placeholder objects** (great for localization-proof, version-proof, focus-directing visuals).
- **Auto-Simplify:** Snagit auto-overlays text/elements with simplified shapes; a **Detail** slider tunes how much is simplified (regenerates on change).
- **Manual:** draw additional simplified objects from quick styles; resize/move/delete individual ones.
- **Color palette system:** save and reuse custom color palettes across multiple simplified images (brand/product consistency).
- **Properties:** fill color, shape, text-object background color, text height, line spacing, alignment; **Show Original** lets you fade in the underlying image for reference via opacity.

### 4.20 Group / Arrange (layering & z-order)

- Objects can be **grouped** and **arranged** in the z-order (bring forward / send backward / to front / to back) via the right-click context menu and arrange controls.
- **Uncertainty:** the exact grouping commands and keyboard shortcuts weren't enumerated in the fetched tool pages; the layering concept is confirmed by the vector/flatten documentation but precise menu labels should be re-verified against the current build.

### 4.21 Flip / Rotate

- Individual vector objects: rotate via the **green rotation handle** (Callout, Shape, Spotlight, etc.).
- Whole-image rotate/flip is available via the **Image** menu (rotate left/right, flip horizontal/vertical).
- **Uncertainty:** exact per-object flip controls vs. image-level flip menu labels weren't fully captured; re-verify in current build.

### 4.22 Color picker / Eyedropper

- Not a standalone canvas tool but a **shared control** inside every color dropdown: click the **Eyedropper** to sample any pixel color from the screen/image and apply it to the current tool's color (fill/outline/text/etc.).

---

## 5. Grab Text & Edit Text (OCR)

- **Grab Text:** Snagit's **OCR** detects text in an image and converts it to editable/copyable text.
  - From a capture: right-click (Win) / control-click (Mac) the image → **Grab Text** (all text), or use the **Selection** tool to isolate a region first (improves accuracy).
  - During capture: **Grab Text** option in the Selection dropdown of the Capture window.
  - **Plain text** (Windows) / **Make Plain Text** (Mac) in the results dialog strips font/color formatting.
- **Edit Text (in place):** select text with the Selection tool → **Edit Text** → double-click to place cursor in the editable overlay → modify font/color/size in properties, replace, add, or delete text directly on the image.

---

## 6. Selection, Move, Snapping & Alignment

- **Canvas Snapping:** automatically aligns objects to each other / to guides as you drag. Toggle via **Image menu → Canvas Snapping** (check/uncheck).
- **Shift-constrain:** holding **Shift** while drawing constrains to perfect squares/circles, straight (H/V) lines/arrows.
- **Snap to Object** (Selection tool): snaps the selection to nearest shape/icon edges.
- **Move/select multiple:** select objects with the Move tool; **Select All (<tool>)** options let you batch-edit all like objects (steps, text, arrows, magnify, pen) from the Properties panel.
- **Alignment:** Canvas Snapping provides automatic edge/center alignment during drag. **Uncertainty:** explicit "align/distribute" menu commands weren't confirmed in fetched docs.

---

## 7. Undo / Redo, Copy / Paste, Duplicate

- **Undo / Redo:** standard (Ctrl/Cmd+Z / Ctrl+Y or Cmd+Shift+Z), also in the Edit menu/toolbar.
- **Copy / Paste:** Ctrl/Cmd+C / V — works on selections and objects; **Copy All** (Share toolbar) copies the flattened image to clipboard.
- **Duplicate:** objects can be copied and pasted to duplicate; pasted images become new editable objects on the canvas.
- **Uncertainty:** a dedicated single-key "Duplicate" shortcut wasn't confirmed in the fetched docs — re-verify (commonly Ctrl/Cmd+D in similar apps, not certain for Snagit).

---

## 8. Windows vs. Mac — Key Differences

| Area | Windows | Mac |
|---|---|---|
| Editable pen/line label | **Create as Vector** | **Create Editable Line** |
| Highlighter shapes / "apply only to text" | Limited | **Shape options + Apply only to text** |
| Spotlight | Via **Selection + Spotlight & Magnify effect** | Dedicated **Spotlight tool** |
| Pen shapes | Multiple shapes | **Circle only** |
| Callout fill gradient | — | **Gradient** option |
| Custom stamp from current image | Import files only | Import files **+ save Snagit image as stamp** |
| Smart Move "Add Narration" | — | **Available** |
| Shadow **angle** control | Yes (on several tools) | Often direction-only |
| Text/Stamp **padding** | Windows | — |
| Effects portability | — | **Mac effects can be lost** when SNAGX opened on Windows |
| Some effects | Page Curl, Color Replacement, Watermark, Spotlight/Magnify | Reflection, Color Filters, Fade |

---

## 9. Open Uncertainties / To Re-verify in Current Build

1. **Eraser** brush size/shape properties — not detailed in fetched docs.
2. **Group / arrange** exact menu commands and shortcuts — concept confirmed, labels not.
3. **Per-object Flip** vs. image-level Flip/Rotate menu specifics.
4. Dedicated **Duplicate** shortcut.
5. Explicit **align/distribute** commands beyond Canvas Snapping.
6. Exact set of **Move-tool z-order** commands (bring to front / send to back wording).

These should be confirmed directly against the latest Snagit (2026) build, since version history pages indicate ongoing UI/tool changes (e.g., Select-All-like-objects, Smart Move, Smart Redact, Simplify color palettes are relatively recent additions).

---

## 10. Sources

Official TechSmith — Editor & layout:
- https://www.techsmith.com/learn/tutorials/snagit/how-to-edit-a-screenshot/ (Explore Snagit Editor)
- https://www.techsmith.com/learn/tutorials/snagit/ (Snagit Tutorials index)
- https://www.techsmith.com/learn/tutorials/section/snagit/snagit-tools (Snagit Tools index)
- https://support.techsmith.com/hc/en-us/sections/200514878-Editor-Library (Editor & Library section)
- https://support.techsmith.com/hc/en-us/articles/360029204112-Turn-Canvas-Snapping-On-or-Off-in-the-Snagit-Editor-Video (Canvas Snapping)
- https://support.techsmith.com/hc/en-us/articles/360029429931-Customize-the-Toolbar-in-Snagit-Video (Customize Toolbar)

Tools:
- https://www.techsmith.com/learn/tutorials/snagit/arrow/ (Arrow)
- https://www.techsmith.com/learn/tutorials/snagit/text/ (Text)
- https://www.techsmith.com/learn/tutorials/snagit/callout/ (Callout)
- https://www.techsmith.com/learn/tutorials/snagit/shape/ (Shape)
- https://www.techsmith.com/learn/tutorials/snagit/line/ (Line)
- https://www.techsmith.com/learn/tutorials/snagit/highlighter/ (Highlighter)
- https://www.techsmith.com/learn/tutorials/snagit/step/ (Step)
- https://www.techsmith.com/learn/tutorials/snagit/stamp/ (Stamp)
- https://www.techsmith.com/learn/tutorials/snagit/pen/ (Pen)
- https://www.techsmith.com/learn/tutorials/snagit/blur/ (Blur)
- https://www.techsmith.com/learn/tutorials/snagit/magnify/ (Magnify)
- https://www.techsmith.com/learn/tutorials/snagit/spotlight/ (Spotlight — Mac)
- https://www.techsmith.com/learn/tutorials/snagit/selection-tools/ (Selection)
- https://www.techsmith.com/learn/tutorials/snagit/remove-image-parts/ (Crop / Cut Out / Fill / Eraser)
- https://www.techsmith.com/learn/tutorials/snagit/move/ (Move + Smart Move)
- https://www.techsmith.com/learn/tutorials/snagit/simplify/ (Simplify)
- https://www.techsmith.com/learn/tutorials/snagit/tools-to-draw-attention/ (Tools to Draw Attention)
- https://www.techsmith.com/learn/tutorials/snagit/add-comments-labels/ (Add Comments/Labels)

Styles / Themes / Effects / Text:
- https://www.techsmith.com/learn/tutorials/snagit/quick-styles-and-favorites/ (Quick Styles & Favorites)
- https://www.techsmith.com/learn/tutorials/snagit/snagit-themes/ (Snagit Themes)
- https://www.techsmith.com/learn/tutorials/snagit/effects/ (Add Effects)
- https://www.techsmith.com/learn/tutorials/snagit/edit-image-text/ (Grab & Edit Text)

Vector / Flatten / editability:
- https://support.techsmith.com/hc/en-us/articles/115002473092-Create-Source-Graphics-for-Future-Editing-in-Snagit-Editor (SNAG/SNAGX editable objects & flattening)
- https://support.techsmith.com/hc/en-us/articles/203732018-Unable-to-Edit-Any-Existing-Objects-in-the-Snagit-Editor (flattening behavior)
- https://support.techsmith.com/hc/en-us/articles/115002031592-Snagit-Grab-Text-Edit-Text-Best-Practices (Grab/Edit Text)

Version history (for "latest version" feature confirmation):
- https://support.techsmith.com/hc/en-us/articles/42674936732685-Snagit-Windows-2026-Version-History
- https://support.techsmith.com/hc/en-us/articles/41975263481613-Snagit-Mac-2026-Version-History

---

*Compiled June 2026. Reflects Snagit as documented on official TechSmith tutorial/help pages at time of research. Some recent features (Smart Move, Smart Redact, Simplify color palettes, Select-All-like-objects) are version-gated; verify availability against the installed build.*
