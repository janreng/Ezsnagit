# Snagit — Library, Share/Outputs, Templates, File Formats, Preferences & Editions

> Research notes on TechSmith Snagit (latest line: Snagit 2025 / 2026 for Windows & Mac). Compiled from official TechSmith sources (support.techsmith.com, techsmith.com tutorials, product/system-requirements pages) plus reputable secondary sources. Where a behavior is uncertain or version-specific, it is flagged inline. Date of research: 2026-06.

---

## 0. Application Information Architecture (Top-Level Navigation)

Snagit is split into two main windows plus a third in-Editor view:

1. **Capture Window** (a.k.a. the "Capture" interface / Capture Widget)
   - The compact window/widget you use to start a capture. Lets you pick **All-in-One / Image / Video / Panoramic / Scrolling / Grab Text** modes, choose region, set effects/share presets, and trigger capture (default global hotkey **Print Screen** on Windows, **Ctrl+Shift+C** on Mac).
   - Can run on Windows startup and minimize to the tray.

2. **Snagit Editor**
   - Where captures land for markup and editing. Contains the **canvas**, the **Tools toolbar** (annotations: callouts, arrows, text, shapes, stamps, step numbers, blur, etc.), the **Quick Styles / Themes** panel, the **Effects** panel, the **Recent Captures Tray** (bottom strip), and the **Share** bar/dropdown (top-right).
   - Top of the Editor has tabs to switch between the **Editor (canvas)** view and the **Library** view.

3. **Library** (a tab at the top of Snagit Editor)
   - The full, persistent history of every capture (images, videos, GIFs) with search, filtering, tags, and flags. See §1.

The Editor menu bar (Windows: **Edit > Editor Preferences**; Mac: **Snagit menu > Settings**) is where all configuration lives (§5).

---

## 1. Library & Recent Captures

### 1.1 Auto-Save Behavior (core concept)

- Snagit **automatically saves every image capture as a `.snagx` file** and **every unsaved video capture as an `.mp4`** into the **Snagit Library**. You do **not** have to manually "Save" to keep a capture — it is persisted automatically, including all edits and annotations.
- This means the Library acts as a complete, auto-saved capture history. Captures stay there until you explicitly delete them.
- Auto-save can be tuned in **Editor Preferences > Library** (Windows) — e.g. *"Automatically save new image captures in the Library"* can be unchecked. Snagit will **warn if auto-save of a `.snagx` file fails**.
- Historically (Snagit 2021 and earlier) the Library distinguished **"Unsaved Captures"** (stored as `.snag` in a hidden **Snagit Datastore** folder) vs **"Saved Captures"** (exported to a user-chosen location/format). The modern model collapses this: everything is auto-saved as `.snagx` in the Library, and "saving" mostly means *exporting* a flattened copy (PNG/JPG/etc.) to a chosen location.

### 1.2 Recent Captures Tray

The Tray is the horizontal strip at the bottom of Snagit Editor.

- **Shows:** recent image captures, video captures, animated GIFs, and other files opened/edited in the Editor. Click a thumbnail to load it onto the canvas.
- **Capacity:** configurable, **1–200 items** (raised to 200 in Snagit 2022). Set via **Editor Preferences > Library** (recent-captures count).
- **Pinning:** click the pin icon to anchor a thumbnail to the **left** side of the tray for quick access; new captures appear to the right of pinned items. Click pin again to unpin. (Pin support added for image, GIF, and video files.)
- **Reordering:** drag-and-drop thumbnails (pinned or unpinned) to rearrange — useful for organizing project assets.
- **Unsaved-changes indicator:** an **orange "sunburst" star icon** marks a file with unsaved edits.
- **Relationship to Library:** the Tray shows only *recent* items; the **Library** is the comprehensive store. Removing/clearing an item from the Tray does **not** delete it — it remains recoverable in the Library.

### 1.3 Library View

- **Open:** click the **Library** tab at the top of Snagit Editor.
- **Contents:** every capture's history — images, videos, animated GIFs.

**Search**
- Free-text search by **filename** via the Search field; results filter live.

**Filter categories (Search pane, auto-generated):**
- **File Type** — Images / Videos / Animated GIFs
- **Date** — grouped by year, month, and specific date created
- **Applications** — every app Snagit captured from (e.g., Chrome, Word)
- **Websites** — every site captured from (URL-aware)
- **Tags** — custom tags you've assigned
- **Flags** — visual markers you assign (searchable)
- **Favorites** — (Mac) manually marked favorites

Click any category to filter; combine with the search field to narrow further.

**Sorting**
- Sort by **date modified, date created, name, or size**. Click a column heading to sort / reverse order.

### 1.4 Tags & Flags (organizing)

- **Tags:** select one or more captures (Ctrl/Cmd+click for multiple) → **Tag** button → type a new tag or pick existing. View all tags via the **Tags** category in the Search pane. Tags group/find captures later.
- **Flags:** visual markers added to a capture so it stands out in the Tray or Library. Flags are filterable/searchable like tags but are a fixed visual-marker set rather than free text.

### 1.5 Managing Captures

- **Open:** double-click a thumbnail to load it in the Editor.
- **Select:** Ctrl/Cmd+click (multiple); Shift+click (range).
- **Delete:** right-click → **Delete** (Windows) / **Move to Trash** (Mac). Multi-select supported.
- **Metadata:** right-click (Windows) / Ctrl+click (Mac) to view capture metadata (date, source app/website).
- **Combine / templates:** right-click captures to **Combine in Template** (multiple) or **Apply Template** (single), or **Create Video from Images** (see §2).

### 1.6 Where Files Are Stored

- Auto-saved captures live in the **Snagit Library / Datastore**, a hidden application-managed folder by default.
- The Library/Datastore location can be **moved** via **Editor Preferences > Library** ("Library location management").
- Exported copies (PNG, JPG, etc.) go to whatever destination folder you choose at export/share time.
- Storing captures to a cloud service (OneDrive/Google Drive/Dropbox) provides backup + multi-device access — but the *native Library* is local.

### 1.7 `.snagx` Auto-Save Format

- `.snagx` is the **cross-platform native** Snagit format that **replaces** the legacy `.snag` (Windows) and `.snagproj` (Mac) formats.
- It preserves the full editable state: **vector-based objects** — text, callouts, stamps, shapes, step numbers, cursors, and pasted images — remain individually editable.
- Cross-platform: a `.snagx` created on Windows opens/edits on Mac and vice-versa.
- Auto-saved into the Library; this is the only format that guarantees re-editability later (see §3 & §4).

---

## 2. Templates / Create Menu

The **Create** button/menu (in the Editor) bundles features that assemble multiple captures into finished assets.

### 2.1 Create Image from Template ("Combine Images")

**Purpose:** combine multiple images into a professional, formatted **visual guide / tutorial layout**. Common uses: side-by-side before/after comparison, explaining a process or concept, or a timeline of events.

**Template types / source:**
- A set of **built-in basic templates** ship with Snagit.
- **More template layouts and styles** are downloadable via the **TechSmith Assets** subscription (and historically via "Download Snagit Templates").
- Templates support options like **titles, themes, automatic step numbering**, and a **footer attribution**.

**Flow A — from scratch:**
1. **Create button > Image from Template** → opens the *Create Image from Template* dialog.
2. Choose a template layout.
3. Configure settings — **image scaling** (*Fill entire drop zone* expands to fill; *Fit entire image* preserves the whole image, may leave empty space) and **footer attribution**.
4. Click **Create** → template opens on the canvas.
5. **Drag images** from the Tray onto image drop zones; double-click + drag to reposition within a zone; add **text/captions** and annotations.

**Flow B — from selected captures:**
- In the Tray or Library, select images → right-click:
  - **Combine in Template** (multiple images) → pick a template, **reorder** thumbnails, add titles/theme, toggle **step numbering**.
  - **Apply Template** (single image).

**Editing an existing template-based image:** you can re-open and modify the layout, swap images, reorder, change theme, etc. (the assembled image stays editable while in `.snagx`).

### 2.2 Create Video from Images

**Purpose:** record **voice narration over one or more screenshots/images**, annotating live as you talk — turning static captures into a short tutorial video (output `.mp4`).

**Flow:**
1. Select one or more images (Ctrl/Cmd+click) in the Tray, **or** **Create > Video from Images**, **or** right-click selection → **Create Video from Images**.
2. The **Recording toolbar** appears: choose background color, cursor, **microphone**, **webcam** options.
3. Start recording — **Shift+F9** (Win) / **Ctrl+Shift+Space** (Mac).
4. While recording, add **arrows, shapes, step numbers, and other annotations** over the images live.
5. Stop — **Shift+F10** (Win) / **Ctrl+Shift+V** (Mac).

**Related cloud/desktop alternatives** (via Share, not the Create menu):
- **Camtasia Online** (web): record up to 5 min of narration per image, ± camera, trim, reorder scenes, apply templates, collaborate, export to Screencast or Camtasia Editor.
- **Camtasia Editor** (desktop): combine images with other screen/camera/mic recordings as separate editable tracks; add captions, callouts, animations, zoom-n-pan; export MP4 or SCORM.

### 2.3 Summary of "Create" Options

| Option | Input | Output |
| --- | --- | --- |
| Image from Template / Combine in Template | Multiple images | One combined image guide |
| Apply Template | Single image | Templated single image |
| Video from Images | One or more images + narration | `.mp4` video |

---

## 3. Share / Outputs

### 3.1 Three sharing methods

1. **Copy / Copy All** → clipboard, paste with Ctrl+V (Win) / Cmd+V (Mac).
2. **Share Link** → uploads to a cloud host (default **Screencast**) and copies a shareable URL.
3. **Share to a specific destination** → pick from the **Share dropdown**.

### 3.2 Share destinations (outputs)

> The exact list and ordering depend on platform and which destinations are enabled in preferences. Below is the union of documented destinations.

**Cloud & collaboration**
- **Screencast** (TechSmith's free cloud hosting / Share Link target)
- **Google Drive**
- **Dropbox**
- **Box**
- **Microsoft OneDrive** (Windows)
- **Microsoft SharePoint** (Windows)

**Microsoft Office**
- **Word**, **PowerPoint**, **Excel**
- **OneNote** (Windows)
- **Outlook** (Mac)

**Communication**
- **Slack** (dedicated Snagit + Slack app integration)
- **Microsoft Teams**
- **Email** (Windows)
- **X / Twitter** (Windows)

**Video / TechSmith ecosystem**
- **YouTube**
- **Camtasia Online**
- **Camtasia Editor** (sends capture directly into Camtasia's clip bin / as tracks)

**Local & other**
- **File** (save to local disk in a chosen format)
- **FTP**
- **Printer** (Windows; not on Windows-on-ARM)
- **Program / Application** (send to a custom external app — see §3.4)
- **Clipboard** (Windows)
- **Evernote** (Windows)

**Apple-specific (Mac)**
- **Pages**, **Keynote**, **Numbers**
- Native macOS share sheet targets: **AirDrop, Mail, Messages, Notes, Photos, Freeform, Journal, Reminders**

### 3.3 Configuring / enabling outputs

- **Windows:** Editor → **Edit > Editor Preferences > Share** tab → check/uncheck a destination to enable/disable; reorder destinations to control their order in the Share dropdown.
- **Mac:** **Snagit menu > Settings > Share** tab → **+** to add a destination from a list, **−** to remove; reorder as needed.
- Per-destination preferences let you set the **account**, **privacy level**, **default folder**, custom **name** (Mac), and **keyboard shortcut** (Mac).

### 3.4 Custom outputs (Program / Application)

- **Windows:** Share dropdown → **Program > Manage > Add Program** → select an `.exe`. Snagit then hands the capture file to that program.
- **Mac:** Share dropdown → **Preferences > Add Share > Application** → select the app's startup file.
- Additional destination plugins can be **downloaded** ("Download Snagit Share Destinations") and, for enterprise, pre-configured via deployment tooling.

---

## 4. File Formats

### 4.1 Native / editable formats

- **`.snagx`** — modern cross-platform native format. Preserves **all vector objects** (text, callouts, stamps, shapes, cursors, pasted images) as editable. **This is the only format that guarantees future re-editability.** Auto-saved by the Library.
- **`.snag`** — legacy Windows native (still listed as a supported save format); `.snagproj` was the legacy Mac project format. Both superseded by `.snagx`.

### 4.2 Image formats — import & export

`BMP, GIF, ICO, JPG, PNG, TIF, RAS, EMF, PSD, TGA, WFX, WMF, WPG` (plus `.snag`/`.snagx`).

### 4.3 Export-only formats

`CUR`, `PDF`, `EPS`. (Multi-page PDF export is supported from multiple captures.) `MHT` is also listed (document/web-archive output).

### 4.4 Video format

- **`.mp4`** (H.264). This is the only video output; unsaved video captures auto-save as `.mp4` in the Library.

### 4.5 Editability rules (important)

- Exporting/sharing to **PNG, JPG, TIFF, GIF, BMP, PDF** etc. **flattens** the image — every annotation/object becomes baked-in pixels and is **no longer editable**.
- On **Mac**, when you quit Snagit, any open **non-`.snagx`** file is **flattened**, and the prior annotations can no longer be edited.
- **Best practice:** complete all edits in `.snagx` first; keep a `.snagx` copy if you may need to re-edit; duplicate before major edits; use transparency-capable formats (PNG/GIF/TIF) if you must export beyond `.snagx`.
- Transparency is supported on PNG, GIF, TIF (not on JPG/BMP).

> Note: The current "What File Formats Does Snagit Support?" support article lists `SNAG` explicitly; `.snagx` is documented as the auto-save/native format across the file-save and version-compatibility tutorials. Treat `.snagx` as the canonical current native format.

---

## 5. Preferences / Settings

Windows: **Edit > Editor Preferences** (Editor) and the **Capture preferences** (from the Capture window). Mac: **Snagit menu > Settings**. Tabs below reflect the Windows layout (Mac groups similar options under Settings).

### 5.1 Capture Preferences (Windows)

- **General:** run Snagit at Windows startup; capture interface (Window vs Widget); **color theme — Light / Dark / System**; automatic update checking.
- **Capture:** hide Snagit during capture; magnifier & crosshairs; scrolling-capture settings; **microphone** and **HDR color** support; **video** options (auto-start, countdown, quality); **webcam** selection & hardware acceleration; printer capture config.
- **Hotkeys:** customizable global/capture shortcuts; option to override conflicts with other apps.

### 5.2 Editor Preferences (Windows)

- **General:** color theme; **language** (6 languages); Capture Widget visibility; onboarding/tooltips; **Data & Privacy**; product help & notifications.
- **Editor:** workspace background color; **thumbnail size** for recent captures; auto-expand canvas for pasted objects; Escape-key behavior; cursor-style preview popup; toolbar labels; spell-check language.
- **Share:** which destinations appear in the Share dropdown and their order; file-naming for shared content (see §3.3).
- **Library:** **Recent Captures Tray count (1–200)**; auto-save settings for images & videos; delete-confirmation prompts; **Library location** management; **backup import**.
- **Advanced:** background Editor operation; delete cropped content; image-flattening preferences; mobile-device file sharing; **factory reset**.
- **Experiments:** beta feature access & feedback.
- **Save As:** remember last-used format vs always use a selected format (this is how you change the default save type away from `.snagx`).

### 5.3 Themes (Light / Dark)

- Two UI themes — **Light** and **Dark** (plus **System** on Windows). Dark offers higher contrast. Set in Capture **General** and/or Editor **General**.

### 5.4 Hotkeys / Keyboard

- Windows: **Hotkeys** tab. Mac: **Settings > Keyboard** tab → click a shortcut → press the new key combination to rebind. Defaults documented in the Snagit Hotkeys Guide.

### 5.5 Updates

- Automatic update checking toggle in Capture > General. Each license year includes free upgrades via **Maintenance** (see §6).

### 5.6 Quick Styles & Themes (annotation presets)

- A **Theme** = a set of custom **Quick Styles** based on a chosen color palette. Quick Styles save a tool's properties (color, size, font) for reuse; themes auto-generate or let you add Quick Styles manually. (This is distinct from the Light/Dark UI theme.)

---

## 6. Editions & Platform

### 6.1 System Requirements (current)

**Windows**
- OS: **Windows 11**, Windows 11 on ARM (23H2+), **Windows 10 (20H2+)**, Windows Server 2016/2019/2022.
- **64-bit only.**
- CPU: Intel i5 6th-gen or AMD equivalent (**Intel 8th-gen+ / AMD Ryzen 2000+** recommended for video).
- RAM: **8 GB recommended.**
- Disk: **1.6 GB** for install.
- Requires **.NET 4.7.2+** and **WebView2 Runtime** (bundled). Windows N editions need the Media Foundation Pack. Printer output unavailable on Windows-on-ARM.

**macOS**
- **macOS Tahoe (26), Sequoia (15), or Sonoma (14)** for the current Snagit line (older Snagit versions support older macOS).

### 6.2 Windows vs Mac feature parity

`.snagx` makes files fully cross-platform, but some integrations differ:

- **Windows-only:** Email output, Evernote, X/Twitter, OneNote, Printer, Program (custom .exe), Clipboard output, OneDrive, SharePoint.
- **Mac-only:** Outlook output, Pages/Keynote/Numbers, native macOS share sheet (AirDrop, Mail, Messages, Notes, Photos, Freeform, Journal, Reminders), per-destination custom names/shortcuts.
- **Behavioral:** on Mac, non-`.snagx` files flatten on quit (§4.5). Settings live under **Snagit menu > Settings** (Mac) vs **Edit > Editor Preferences** + Capture prefs (Windows).
- Core capture/markup/template/Library functionality is broadly equivalent across platforms.

### 6.3 Licensing

- **Personal subscription** (~$39/yr at time of research) — non-transferable; always latest version.
- **Perpetual + Maintenance:** a perpetual license tied to a version, with **one year of Maintenance** included per purchase → free upgrade to the next major version while active, plus live support and the Snagit Certification course. (TechSmith has shifted emphasis toward subscriptions but still offers perpetual/volume options.)
- **Business subscriptions:** transferable to another user.
- **Volume licensing:** discounts from **5+ licenses**; enterprise deployment tooling for MSI/admin config.

### 6.4 Relationship to Camtasia & Screencast

- **Screencast** (screencast.com): TechSmith's cloud hosting; default **Share Link** target; gives a shareable URL with viewer comments/feedback. Free tier exists.
- **Camtasia:** TechSmith's video editor. Snagit can hand captures to **Camtasia Editor** (clip bin / editable tracks) or **Camtasia Online** for richer video production. Snagit's own "Video from Images" is the lightweight path; Camtasia is the heavyweight path. Snagit + Camtasia are positioned as a complementary capture→edit pipeline, both feeding Screencast for sharing.

---

## 7. Uncertainties / Notes

- **Version numbering:** TechSmith uses year-based versions (Snagit 2024/2025/2026). The system-requirements page does not print a single "latest version" number; OS support (macOS Tahoe/Sequoia/Sonoma; Win 11/10) implies the 2025–2026 line.
- The current "What File Formats" support article lists **`SNAG`** explicitly; `.snagx` is documented elsewhere as the auto-save/native format. Both are accurate — `.snagx` is the modern default.
- Exact **Recent Captures Tray default count** vs the older 200 maximum may vary slightly by version; the configurable range 1–200 is current.
- Share-destination availability can be changed by admins/enterprise config, so a given install may show a subset.
- Pricing is indicative and region/promotion-dependent.

---

## Sources

- [Snagit Library, Saved vs Unsaved Captures (2021 and earlier) — TechSmith Support](https://support.techsmith.com/hc/en-us/articles/203732798-Snagit-Library-Saved-Captures-Versus-Unsaved-Captures-Snagit-2021-and-Earlier)
- [Introduction to Snagit Library — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/snagit-library/)
- [Recent Captures Tray — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/recent-captures-tray/)
- [How to Default to a File Type Other Than SNAGX — TechSmith Support](https://support.techsmith.com/hc/en-us/articles/38927872972941-How-to-Default-to-a-File-Type-Other-Than-SNAGX)
- [Automatically Save Images to a Specific Format During Capture — TechSmith Support](https://support.techsmith.com/hc/en-us/articles/203731148-Automatically-Save-Images-to-a-Specific-Format-During-Capture)
- [Create Visual Guides with Templates (Image from Template) — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/image-from-template/)
- [Edit a Template — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/modify-template/)
- [Combine Images — Snagit Feature](https://www.techsmith.com/snagit/features/combine-images/)
- [3 Ways to Turn Screenshots into a Video (Video from Images) — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/video-from-images/)
- [Share Captures — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/share/)
- [Get a Shareable Link — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/share-link/)
- [Connect and Collaborate with Snagit + Slack — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/snagit-slack-app/)
- [Download Snagit Share Destinations — TechSmith Support](https://support.techsmith.com/hc/en-us/articles/115002022732-Download-Snagit-Share-Destinations)
- [Are Snagit and Camtasia Editor Compatible With Other Enterprise Tools? — TechSmith Support](https://support.techsmith.com/hc/en-us/articles/41687247503757-Are-Snagit-and-Camtasia-Editor-Compatible-With-Other-Enterprise-Tools)
- [Upload Content to Screencast — Tutorial](https://www.techsmith.com/learn/tutorials/screencast/upload-to-screencast/)
- [What File Formats Does Snagit Support? — TechSmith Support](https://support.techsmith.com/hc/en-us/articles/41688493993613-What-File-Formats-Does-Snagit-Support)
- [Best Practices for Saving Your Snagit Files — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/file-save-tips/)
- [Create Source Graphics for Future Editing in Snagit Editor — TechSmith Support](https://support.techsmith.com/hc/en-us/articles/115002473092-Create-Source-Graphics-for-Future-Editing-in-Snagit-Editor)
- [Snagit File Format Compatibility — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/version-compatibility/)
- [Snagit Preferences (Windows) — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/preferences/)
- [Snagit Settings (Mac) — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/preferences-mac/)
- [Customize Snagit — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/customize-snagit/)
- [Snagit Hotkeys Guide — Tutorial](https://www.techsmith.com/learn/tutorials/snagit/snagit-hotkeys/)
- [What Are the System Requirements for Snagit? — TechSmith Support](https://support.techsmith.com/hc/en-us/articles/41688606378637-What-Are-the-System-Requirements-for-Snagit)
- [System Requirements — Snagit Product Page](https://www.techsmith.com/snagit/system-requirements/)
- [Buy Snagit — TechSmith Store](https://www.techsmith.com/store/snagit)
- [Enterprise Install Guidelines for Snagit on macOS — TechSmith Support](https://support.techsmith.com/hc/en-us/articles/115007344888-Enterprise-Install-Guidelines-for-Snagit-on-MacOS)
- [SNAGX File — FileInfo](https://fileinfo.com/extension/snagx)
- [SNAG File — FileInfo](https://fileinfo.com/extension/snag)
