# Snagit Capture Features — Research (Clone Spec Input)

> Research compiled from TechSmith official sources (support.techsmith.com help articles, techsmith.com/learn tutorials, and techsmith.com/snagit product/feature pages). Latest Snagit (Windows 2024+ / Mac 2024+ generation). Where a behavior is platform-specific, it is called out. Items marked **(uncertain)** could not be fully confirmed from a single authoritative page and should be verified against a live build before implementing.

---

## 1. Overview: Capture Window, Capture Modes, and Entry Points

Snagit's capture experience is driven by the **Capture Window** plus a set of **capture modes** and an **on-screen capture process** (crosshairs → selection → on-screen toolbar → finalize).

### 1.1 Three top-level capture modes (tabs)

The Capture Window exposes three capture modes as tabs:

| Tab | Purpose |
|-----|---------|
| **All-in-One** | Pre-selected default settings to quickly grab an **image or video** on the fly. Lets you decide image vs. video vs. scrolling **at the moment of capture** (after dragging a selection), rather than committing beforehand. |
| **Image** | Customizable **image** capture. Add automatic effects, share to a destination, set a time delay, choose a specific Selection type (Region, Window, Fullscreen, Grab Text, Advanced modes, etc.). |
| **Video** | Customizable **video** recording. Configure audio (system/mic) and a webcam device. |

> Naming note: TechSmith documents "3 Capture modes: All-in-one, Image, and Video." On some marketing/legacy pages the third toggle in the on-screen flow is described as **Image / Video / Panoramic**; in current builds "Panoramic" capture is surfaced as **Scrolling capture** (see §5). Historically: "Panoramic" → renamed **"Scrolling"**, and the old "Scrolling" (auto) → renamed **"Automatic Scrolling."**

### 1.2 Four ways to start a capture

TechSmith documents four entry points (article "4 Ways to Start a Capture"):

1. **Capture Window** — Click the **Capture button** on any tab to start a capture using that tab's current settings.
2. **Hotkeys** — Global hotkey (default **Print Screen** on Windows / **Control+Shift+C** on Mac) starts a capture using the current Capture Window settings, from any app, while Snagit runs in the background. Preset-specific hotkeys can also be assigned.
3. **Presets** — Saved capture configurations; each can have its own dedicated hotkey (see §8).
4. **OneClick** (Capture Widget) — A floating tab/toolbar at the screen edge that expands on hover for quick preset-based capture.

The capture interface itself can be set (in Preferences) to either the **Capture Window** or the **Capture Widget (OneClick)**.

---

## 2. The All-in-One Capture: Window, Button, Crosshairs, Magnifier, Dimensions

### 2.1 What it is

All-in-One is the default, fastest capture path. You start the capture, drag out a region, and then choose what kind of capture to make (image, video, or scrolling) from an **on-screen toolbar** that appears beside/under your selection.

### 2.2 UX flow (Windows; Mac equivalent in parentheses)

1. Open the Capture Window (taskbar/menu-bar icon, or the **Capture** button in Snagit Editor).
2. Select the **All-in-One** tab.
3. Click the **Capture** button (or press **Print Screen** / Mac **Control+Shift+C**).
4. The screen dims slightly and **orange crosshairs** appear, locked to the cursor; a **magnifier** loupe and a **dimension readout** (pixel width × height, and/or cursor coordinates) follow the cursor for pixel-precise placement.
   - Hovering a window/UI element triggers **automatic region detection**: an **orange dashed border** snaps around the detected window/control. Click to capture that detected region.
   - Or **click and drag** to define a custom rectangular region; release to set it. While dragging, the dimension readout shows the live width × height.
5. On release (or window-click), the **on-screen capture toolbar** appears next to the selection. From it you choose Image / Video / Scrolling (and access Grab Text / effects / dimension fields, depending on build — see §6).
6. Finalize (e.g., click the **Image** capture button) → the capture opens in **Snagit Editor** to preview, edit, and share. (Or it goes straight to a configured share destination if set.)

### 2.3 Crosshairs / Magnifier / Dimensions

- **Orange crosshairs**: indicate the active capture state; the cursor becomes a crosshair for precise selection.
- **Magnifier**: a zoomed loupe near the cursor for pixel-accurate edge selection. Can be toggled via the **Show magnifier** preference. A **Simplified crosshairs** preference streamlines the cursor for performance.
- **Dimension readout**: live width × height (pixels) while dragging; exact dimensions can also be set numerically via the **Selection properties** control (fixed region size + position coordinates).

---

## 3. Capture Modes / Selection Types (Image tab → Selection dropdown)

In the **Image** tab, the **Selection** dropdown picks what gets captured. Core options plus an **Advanced** submenu:

### 3.1 Region / Selection
- **What:** Capture a user-drawn rectangular portion of the screen.
- **Flow:** Image tab → Selection = **Region** → click **Capture** (or Print Screen) → **click and drag** to select the area → release to capture.

### 3.2 Window
- **What:** Capture a single open window (auto-detected).
- **Flow:** Selection = **Window** → **Capture** → hover the target window until an **orange dashed border** appears → click to capture that window.

### 3.3 Full Screen
- **What:** Capture the entire screen.
- **Flow:** Selection = **Fullscreen** → **Capture** (or hotkey).
- **Multi-monitor:** Fullscreen captures **only one monitor** at a time (the monitor under the cursor / active monitor).

### 3.4 Scrolling Capture
- **What:** Capture content longer/wider than the visible window (full web pages, long docs, wide spreadsheets). Covered in detail in §5.

### 3.5 Grab Text (OCR)
- **What:** OCR — extract editable text from a screen region.
- **Flow:** Selection = **Grab Text** → **Capture** (Print Screen / Mac Shift+Ctrl+C) → **click and drag** the area → the **Grab Text Results** dialog opens in Editor with the recognized text → **Copy All** / copy to paste elsewhere.
- Can be combined with scrolling capture to turn a flat spreadsheet image into editable data.

### 3.6 Advanced → Menu Capture
- **What:** Capture an open drop-down/cascading menu (which would normally disappear when you click elsewhere).
- **Flow:** Selection = **Advanced > Menu** → (Windows) optionally click **Menu Properties** to configure sub-menu/menu options → optionally enable **Time Delay** → **Capture** → open the target menu during the countdown so it's captured.

### 3.7 Advanced → Timed / Delay (and Schedule / Interval)
- **What:** Delay capture by N seconds (to set up the screen), schedule at a date/time, or capture at repeating intervals (time-lapse).
- **Flow:** Pick a Selection type → enable **Time Delay** → choose **Delay** and enter seconds → **Capture** (or Print Screen) → a **countdown appears in the bottom-right corner** before capture fires.
- Sub-options of the Time Delay control:
  - **Delay** — seconds before capture.
  - **Schedule** — capture at a specified date/time.
  - **Interval** — time-lapse captures at regular continuing intervals. **(Windows only — "Time-lapse capture")**

### 3.8 Advanced → Multiple Area
- **What:** Capture several non-contiguous regions/windows in one combined capture. **(Windows feature; Mac achieves this via modifier key, below.)**
- **Windows flow:** Selection = **Advanced > Multiple Area** → **Capture** → **orange crosshairs** appear → click windows and/or drag regions to add each one → click the **Finish** button at the top of the screen.
- **Mac flow:** All-in-One tab → **Capture** → **hold Command** and drag to select multiple areas → release Command to finish.

### 3.9 Advanced → Freehand
- **What:** Capture an irregular, hand-drawn shape (non-rectangular). **(Windows)**
- **Flow:** Selection = **Advanced > Freehand** → **Capture** → cursor becomes a **scissors** icon → **click and drag** around the area to outline it → release to capture the enclosed shape.

### 3.10 Capture from Camera / Webcam
- **What:** Capture a still image from a built-in webcam or connected camera. **(Documented as Mac-only for image webcam capture: "Webcam capture — Capture images from your webcam (Mac only).")**
- **Mac flow:** Image tab → Selection = **Webcam** → **Capture** → select the camera device from the dropdown → activate capture.
- On both platforms, webcam is also used for **Picture-in-Picture video** (screen + webcam simultaneously) — see Video research; not strictly an image-capture mode.
- Mac Selection dropdown also includes **Mission Control** as a capture target.

### 3.11 Printer Capture **(Windows only)**
- **What:** Send content into Snagit Editor via an application's **Print** option (Snagit appears as a printer). Advertised as "Printer capture." Detailed flow not covered here — verify against build.

---

## 4. The On-Screen Capture Toolbar (the "orange toolbar")

After you select a region (drag-release, or window auto-detect), Snagit shows an on-screen toolbar **before** the capture is finalized. This is the key decision point for All-in-One.

**Confirmed buttons / controls:**
- **Image Capture** button — finalize as a still screenshot.
- **Video Capture** button — switch into video recording of the selected region (opens video options/countdown).
- **Scrolling Capture** button — start a scrolling/panoramic capture of the selected window (see §5).

**Likely-present controls (documented elsewhere in the capture flow; verify exact placement on this toolbar):**
- **Grab Text** option (OCR of the selection). **(uncertain — placement on the on-screen toolbar vs. only via Selection dropdown)**
- **Dimensions / Selection properties** readout & numeric entry (exact width × height, snap-to-window). **(uncertain placement)**
- **Effects** quick-apply. **(uncertain placement)**
- **Cancel** (Esc).

For **scrolling**, when you hover a scrollable window during selection, **directional scrolling arrows** appear directly on/near the selection (horizontal, vertical, or both) — these are part of this on-screen step (see §5.2).

> Note: TechSmith's own tutorial text emphasizes the three primary toggles (Image / Video / Scrolling) and does not publish an exhaustive button inventory for this intermediate toolbar; the remaining items above are inferred from the documented capture flow and should be confirmed in a live build.

---

## 5. Scrolling Capture (incl. former "Panoramic")

Snagit offers **two scrolling methods**: **Automatic Scrolling** (arrows) and **Manual / Panoramic** (you scroll).

### 5.1 Automatic Scrolling (scrolling arrows)
- **What:** Snagit detects a scrollable window and auto-scrolls + stitches the full content. Hands-off.
- **Arrows that appear on hover** (if the app supports auto-scroll):
  - **Horizontal arrow** — left/right; captures full width.
  - **Vertical arrow** — up/down; captures full height (long page/doc).
  - **Both / diagonal arrow** — captures the **entire page** (full width + height).
- **Flow:** Open the content → All-in-One (or Image) tab → **Capture** → position cursor over the window → **click the appropriate arrow** → Snagit auto-scrolls and assembles the capture, then opens it in Editor.
- **Hotkeys (Windows):** **H** = horizontal, **V** = vertical, **B** = both/diagonal. **(Mac: R = horizontal, D = vertical, X = diagonal.)** **(uncertain — confirm Mac letters per build)**

### 5.2 Manual / Panoramic Scrolling Capture
- **What:** You control the scroll, capturing only the portion you want — more precise than auto-scroll; works where auto-scroll fails (e.g., some parallax sites).
- **Flow (current build):**
  1. All-in-One tab → **Capture** (or Print Screen / **Ctrl+Shift+C**).
  2. **Drag to select an area** within the window.
  3. Click the **Scrolling Capture** button in the on-screen toolbar.
  4. **Manually scroll** down / up / side-to-side using the window's **scroll bars** (preferred) or the mouse wheel.
  5. Click **Done** when finished → the stitched image opens in Editor.
- **Guidance from TechSmith:** scroll **smoothly** (not too fast/slow); prefer **scroll bars over the mouse wheel** to avoid capturing hover states; preview/control bar should not be included in the capture.
- **Preference:** **"Require Start button for scrolling capture"** toggles whether scrolling begins automatically or waits for an explicit Start (Windows and Mac).

### 5.3 Full-page capture
- Selecting the **both-directions** arrow (auto) or scrolling through the whole page (manual) yields a full-page screenshot — entire web page / long document / wide spreadsheet in one image.

---

## 6. Capture Settings, Presets, and Share Destinations

### 6.1 Per-tab capture settings
Within each tab you configure: Selection type, **Capture Cursor** (include/exclude pointer), **Time Delay**, automatic **Effects**, **Preview in Editor** (on/off), **Copy to Clipboard**, and a **Share** destination.

### 6.2 Presets
- **Presets** are saved capture configurations: capture type + selection + cursor + delay + effects + share destination, all bundled.
- Each preset can be triggered from the Capture Window and can be assigned its own **dedicated hotkey**.
- Examples TechSmith cites: a scrolling-capture preset; a "video with webcam overlay" preset; a "copy to clipboard" preset; a "save as JPG/PNG" preset.
- Advertised as **"Custom capture presets"** + **"Custom keyboard shortcuts"** (shortcut per preset).

### 6.3 Share destination per profile/preset
- A preset can include a **Share** output (e.g., clipboard, file, or an integrated destination) so the capture is routed automatically on finalize.

### 6.4 OneClick defaults (fixed)
When capturing via **OneClick** widget, settings are fixed: Capture Type **All-in-One**, **Preview in Editor: On**, **Capture Cursor: On**, **Delay: None**, **Effects: None**, **Share: None**. (OneClick does **not** inherit the Capture Window's current settings — a documented gotcha.)

---

## 7. Capture Preferences (Cursor, Delays, Monitors, Magnifier, Scrolling)

From the Windows **Preferences** and Mac **Settings** (capture-relevant items):

**Windows (Preferences):**
- **Run Snagit when Windows starts.**
- **Capture interface**: Capture Window vs. Capture Widget (OneClick).
- **Show magnifier** — pixel-accurate selection loupe.
- **Simplified crosshairs** — streamlined cursor for performance.
- **Require Start button for scrolling capture** — auto vs. manual scroll start.
- **Support HDR color settings.**
- **Hotkeys tab** — change hotkey commands; **Allow Snagit to override other hotkey assignments** to avoid conflicts.
- **Capture cursor** — include or exclude the mouse pointer in captures (also per-tab/preset).

**Mac (Settings):**
- **Hide Snagit and controls when capturing** — keep Snagit UI out of the capture.
- **Hide desktop icons during video capture.**
- **Require Start button for scrolling capture.**
- **Keyboard tab** — customizable hotkeys.
- Webcam device selection; video frame rate / encoding (video-side).

**Multiple monitors:**
- **Fullscreen** captures a **single monitor** only (the active/cursor monitor).
- Region/Window/Scrolling work across the monitor under the cursor; spanning monitors for a single region is not a documented headline feature — **(verify)**.

**Delays:**
- Time Delay (seconds), Schedule (date/time), Interval/time-lapse (Windows). Countdown shows bottom-right of screen.

---

## 8. Global Hotkeys / Shortcuts & PrintScreen Integration

### 8.1 Default Windows capture shortcuts (from official "Keyboard Shortcuts for Windows")
| Action | Shortcut |
|--------|----------|
| **Global Capture** | **Print Screen** |
| **Start a Region Selection** | **Shift + Enter** |
| **Take a Capture** (confirm/finalize) | **Enter** |
| **Cancel Capture** | **Esc** |
| **Start / Pause / Resume Recording** | **Shift + F9** |
| **Stop Recording** | **Shift + F10** |
| Scrolling: Horizontal / Vertical / Both | **H / V / B** (during capture, on hover) |
| Repeat Last Capture | (documented; verify exact Windows key) **(uncertain)** |

### 8.2 Default Mac capture shortcuts
| Action | Shortcut |
|--------|----------|
| **Global Capture** | **Control + Shift + C** |
| **All-in-One** | **Control + Shift + O** |
| **Image Capture** | **Control + Shift + S** |
| **Video Capture** | **Control + Shift + V** |
| **Take Capture** (finalize) | **Command + Return** |
| **Cancel** | **Esc** |
| **Fullscreen** | **F** |
| **Grab Text** | **Shift + Command + O** |
| **Repeat Last Capture** | **Control + Shift + R** |
| Scrolling: Horizontal / Vertical / Diagonal | **R / D / X** |
| Start/Pause Recording | **Control + Shift + Spacebar** |

> The global capture hotkey is editable: open the Capture Window, click the **Shortcut** field, and press the desired key combination. Preset hotkeys are set per preset. Full list in the official **Snagit Hotkeys Guide**.

### 8.3 PrintScreen integration (Windows)
- Snagit can bind to the **Print Screen** key as the global capture trigger.
- **Conflict on Windows 11:** Print Screen may open the native **Snipping Tool** instead. To give the key to Snagit, disable Windows **Settings → Accessibility → Keyboard → "Use the Print screen key to open screen capture,"** then restart.
- A dedicated option/article exists for **"Capture Directly to Image Using Print Screen"** (Print Screen produces an image capture directly).

---

## 9. Windows vs. Mac Capture Differences (summary)

| Aspect | Windows | Mac |
|--------|---------|-----|
| Global capture hotkey | Print Screen | Control+Shift+C |
| Tabs | All-in-One / Image / Video | All-in-One / Image / Video |
| Multiple Area | **Advanced > Multiple Area** + Finish button | Hold **Command** + drag multiple, release Command |
| Freehand | Yes (Advanced > Freehand) | **(uncertain — verify Mac availability)** |
| Time-lapse / Interval | Yes (Windows only) | No |
| Webcam **image** capture | Not documented (PiP video only) | **Yes** (Selection > Webcam) |
| Printer capture | **Yes** (Windows only) | No |
| Mission Control capture | No | Yes (Selection option) |
| Scrolling arrow hotkeys | H / V / B | R / D / X |
| Menu capture properties | Menu Properties dialog | Simpler / differs |
| Fullscreen shortcut | (via Selection/hotkey) | **F** |

---

## 10. Open Items to Verify Against a Live Build
- Exact button inventory and ordering on the **on-screen (orange) toolbar** (Grab Text / Effects / Dimensions placement).
- Whether the third on-screen toggle is labeled "Panoramic" or "Scrolling" in the current build.
- Mac availability of **Freehand** and **Menu** advanced modes.
- Exact **Repeat Last Capture** key on Windows.
- Multi-monitor behavior for a single spanning region capture.
- Whether "All-in-One / Image / Video" appear as the on-screen toggle vs. only the Capture Window tabs.

---

## Sources

- All-in-one, Image, and Video Capture Modes Explained (Video) — https://support.techsmith.com/hc/en-us/articles/360029313392-All-in-one-Image-and-Video-Capture-Modes-Explained-Video
- Capture Window Basics in Snagit (Video) — https://support.techsmith.com/hc/en-us/articles/360029204232-Capture-Window-Basics-in-Snagit-Video
- 4 Ways to Start a Capture in Snagit (Video) — https://support.techsmith.com/hc/en-us/articles/360030582052-4-Ways-to-Start-a-Capture-in-Snagit-Video
- Capture Modes in Snagit Windows — https://support.techsmith.com/hc/en-us/articles/360002743452-Capture-Modes-in-Snagit-Windows
- Common Captures with Snagit (Tutorial) — https://www.techsmith.com/learn/tutorials/snagit/common-captures/
- Capture a Scrolling Area (Tutorial) — https://www.techsmith.com/learn/tutorials/snagit/scrolling-capture/
- Capture Your Screen (Tutorial) — https://www.techsmith.com/learn/tutorials/snagit/how-to-capture-your-screen/
- Capture an Image (Tutorial) — https://www.techsmith.com/learn/tutorials/snagit/image-capture/
- Snagit Hotkeys Guide (Tutorial) — https://www.techsmith.com/learn/tutorials/snagit/snagit-hotkeys/
- Snagit Capture Hotkeys / Change the Global Hotkey (Tutorial) — https://www.techsmith.com/learn/tutorials/snagit/change-global-hotkey/
- Keyboard Shortcuts for Windows (PDF) — https://www.techsmith.com/blog/wp-content/uploads/2024/02/Snagit-Shortcuts-for-Windows.pdf
- Snagit Preferences - Windows (Tutorial) — https://www.techsmith.com/learn/tutorials/snagit/preferences/
- Snagit Settings - Mac (Tutorial) — https://www.techsmith.com/learn/tutorials/snagit/preferences-mac/
- Grab & Edit Text (Tutorial) — https://www.techsmith.com/learn/tutorials/snagit/edit-image-text/
- Snagit Grab Text & Edit Text Best Practices — https://support.techsmith.com/hc/en-us/articles/115002031592-Snagit-Grab-Text-Edit-Text-Best-Practices
- Capture Directly To Image Using Print Screen in Snagit — https://support.techsmith.com/hc/en-us/articles/204189118-Capture-Directly-To-Image-Using-Print-Screen-in-Snagit
- Snagit Features (Capture) — https://www.techsmith.com/snagit/features/
- Record a Picture-in-Picture Video (Tutorial) — https://www.techsmith.com/learn/tutorials/snagit/picture-in-picture/
- Snagit Help 2024 (PDF) — https://www.techsmith.com/learn/wp-content/uploads/2025/01/Snagit-Help-2024-EN.pdf
- Snagit Help 2022 (PDF) — https://www.techsmith.com/blog/wp-content/uploads/2022/10/Snagit-Help-2022.pdf
