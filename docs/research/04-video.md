# Snagit — Video Capture & Video Editing (Research)

> Scope: Video **capture** (recording) and video **editing** features of TechSmith Snagit (latest version, researched June 2026).
> Primary sources: TechSmith official tutorials and support docs (`techsmith.com`, `support.techsmith.com`).
> Notes on confidence: Where a behavior is platform-specific or where official docs were ambiguous, this is flagged inline. Items marked **(uncertain)** could not be fully confirmed from official sources.

---

## 0. Overview

Snagit has two surfaces relevant to video:

1. **Capture window** (the floating capture UI) — where you choose the **Video** tab, configure inputs (camera, mic, system audio, cursor, screen draw), select a region/window/fullscreen, and start a recording. Also hosts **Video from Images**.
2. **Snagit Editor** — where recorded/imported videos open for trimming, cutting, saving frames, exporting to GIF, combining clips, and sharing.

Snagit's video model is a **single-track, lightweight editor** (trim/cut/combine) — not a multitrack NLE. For multitrack editing TechSmith funnels users to **Camtasia** (you can choose to open a Snagit video directly in Camtasia Editor).

Supported video formats in the Editor: **MP4, MOV, and animated GIF**, plus videos recorded inside Snagit itself. Primary export format is **MP4** (and GIF).

---

## 1. Video Capture (Recording)

### 1.1 Starting a recording — UX flow

1. Open the **Capture window** and select the **Video** tab.
2. Adjust the Video-tab settings (see §1.2) — camera, microphone, system audio, cursor, screen draw, "Open In", "Selection", "Share".
3. Click the **Capture** button, or press the global capture hotkey:
   - **Windows:** `Print Screen`
   - **Mac:** `Control+Shift+C`
4. **Orange crosshairs** appear for selecting the recording area:
   - **Click + drag** to draw a custom region.
   - Click **Fullscreen** to record the entire screen.
   - In **Window** selection mode the selection snaps to application window boundaries.
   - You can fine-tune by dragging the selection handles or entering exact **Width / Height** dimensions.
   - **Aspect-ratio lock while dragging:** hold `Control+Shift` (Windows) / `Shift` (Mac) to lock to **16:9**. Docs also reference a **4:3** lock on Windows. Standard dimension guides surfaced: 1920×1080, 1280×720, 640×480, 320×240.
5. After the area is set, a **"Ready to Record" window** (the recording toolbar) appears with the record controls and input toggles.
6. Press **Record** to begin (see hotkeys below). A **countdown** plays before capture begins (3-2-1 style; exact duration not documented — **(uncertain)** on whether it's configurable).
7. Press **Stop** when finished. The video opens in **Snagit Editor** (or the destination chosen in "Open In").

### 1.2 Video-tab settings (Capture window)

- **Open In:** where the video opens after recording — **Snagit Editor** (default, for review/editing), **Camtasia Editor** (multitrack editing), or **None** (saves to Library without opening).
- **Selection:** **Region**, **Window**, or **Fullscreen**. (Mac's Window option follows the selected app even if it's resized.)
- **Share:** pick an immediate sharing destination, or decide later.
- Input toggles, each with a dropdown for additional options: **Camera**, **Screen Draw**, **Cursor**, **Microphone**, **System Audio**.

### 1.3 Recording toolbar / controls

The "Ready to Record" toolbar exposes:

| Control | Windows | Mac |
|---|---|---|
| **Record** | `Shift+F9` | `Control+Shift+Space` (one source also lists `Command+Shift+Space` — **(uncertain)**, likely version-dependent) |
| **Pause** | `Shift+F9` (same as record, toggles) | `Control+Shift+Space` (toggles) |
| **Stop** | `Shift+F10` | `Control+Shift+V` |

- A **timer / elapsed-time** display shows recording duration.
- Inputs can be **toggled on/off both before and during** the recording by clicking the matching icon (e.g., mute the mic mid-recording).
- Toolbar can be repositioned; on dual monitors move it to the non-recording screen. For single-monitor fullscreen recordings, manage the toolbar via hotkeys.

### 1.4 Audio: system audio + microphone

- **System Audio** — records sound produced by the computer (app alerts, audio from speakers / a webinar / Zoom call audio).
- **Microphone Audio** — records from the built-in mic or a connected USB microphone.
- Both can be recorded **simultaneously**.
- **Microphone Options** dropdown: select the input device and enable **background noise removal**.
- You can record both a microphone and incoming/system audio together (the documented webinar/Zoom workflow).

### 1.5 Cursor capture

- **Cursor** toggle controls whether the mouse pointer appears in the recording.
- Cursor effects available from the toolbar/options:
  - **Highlight Cursor** — emphasis ring around the pointer.
  - **Click Animation** — visual feedback on mouse clicks.

### 1.6 Webcam capture & switching screen ↔ webcam (Picture-in-Picture)

- **Camera/Webcam** toggle records live video from a built-in or connected webcam.
  - **Windows** labels it "Record Webcam"; **Mac** labels it "Camera".
- Two display modes:
  - **Picture-in-Picture (PiP)** — webcam overlaid in a corner of the screen recording.
  - **Fullscreen Webcam** — webcam fills the frame.
- **Switch between PiP and Fullscreen Webcam during recording:** click the **Fullscreen Webcam** / **Picture-in-Picture** button, or press **`Control+Shift+F`**.
- **PiP customization** (Camera Options dropdown):
  - **Placement:** Top-left, Top-right, Bottom-left, Bottom-right.
  - **Shape:** Circle, Square, Rounded Square, Rectangle, Rounded Rectangle.
  - **Size:** drag a corner of the webcam preview in/out (works during preview and recording).
  - **Reposition:** click-drag the webcam preview between corners.
- **Mac-only camera options:** **Mirror** (horizontal flip), **Border** with custom color, **Virtual background / blur** (background replacement). On **Intel Macs**, background options are limited (blur/none only — full virtual backgrounds unavailable).

### 1.7 Screen Draw (drawing while recording)

- **Screen Draw** lets you annotate **on screen during recording** to highlight information — shapes, arrows, step numbers.
- A **fade duration** (in seconds) can be set so drawn annotations disappear after a time.
- Available from the Video-tab toggle and the recording toolbar.

### 1.8 Recording tips / limits (from official docs)

- TechSmith recommends **avoiding recordings longer than ~1 hour**; instead record shorter clips and **combine** them in the Editor.
- Enable **Capture Cursor** + highlighting for interactive/how-to content.
- Hard max recording length is **not documented** — **(uncertain)**.

---

## 2. Video Editing in Snagit Editor

When a video opens, the Editor shows the video on the **canvas** with a **playback toolbar** and a **timeline** below.

### 2.1 Timeline, scrubber & playhead

- The **white playhead** controls which frame of the video is displayed.
- **Drag the playhead** along the timeline to scrub/jump to any point (fast-forward/rewind).
- **Frame-by-frame navigation:** **Next Frame** / **Previous Frame** buttons, or the **Left/Right arrow keys**, move exactly one frame.
- **Selection handles:** a **green Start handle** and a **red End handle** sit on the timeline. Drag them to select a segment, used for Cut, Save-as-clip, and GIF creation.
- Standard playback controls: **Play/Pause**, Previous/Next Frame, **volume/mute**, and an **elapsed-time / total-duration** readout.

### 2.2 Trim & Cut Out (remove start / end / middle)

**Trim (start or end):**
1. Position the **Start** and **End** selection handles around the segment to remove.
2. (Optional) Drag the playhead across the selection to **preview** what will be removed.
3. Click **Cut Out**.
4. A **yellow line** on the timeline marks where a segment was removed.

**Cut Out (middle section removal):**
1. Drag the **Start** handle to the beginning of the unwanted middle segment.
2. Drag the **End** handle to the end of that segment.
3. Click **Cut Out** — the middle is removed and the remaining halves join; a **yellow line** marks the join.

> Exact button label: **"Cut Out"**.

### 2.3 Save a clip as a new video

Used to split a long recording into shorter pieces:
1. Drag the **green Start** and **red End** handles to select a portion.
2. Use the playhead to review the selection.
3. Click **Save As New Video** (a.k.a. "Save selection as new video").
4. The new clip opens on the canvas as a separate capture.

### 2.4 Combine / Add Videos

- Click **Add Videos** in the playback toolbar to merge multiple clips from the Snagit **Library** into one video.
- Clips can be **reordered via drag-and-drop**.
- This is the recommended way to assemble several short recordings into one longer video.

### 2.5 Save Frame as Image (extract a still)

1. Drag the **playhead** to the exact frame you want (use frame-by-frame for precision).
2. Click **Save Frame as Image** (camera icon).
3. The frame opens as a **new image** on the canvas — ready to annotate, edit, and share like any Snagit screenshot.
- Works on Snagit recordings and on imported **MP4 / MOV / GIF**. You can also use Snagit's quick-capture hotkeys to grab a frame from video playing in a browser or another app.

---

## 3. Create Animated GIF from Video

### 3.1 UX flow

1. Open a video capture (or pick one from **Recent Captures**).
2. Drag the **green** and **red** handles to select the segment to convert (or choose the whole video later).
3. Preview by dragging the playhead to the green handle and clicking **Play**.
4. Click **Save As Animated GIF** (the **GIF** button).
5. Choose **selection** vs **entire video**.
6. Pick an **optimization preset** (or Custom) in the **Create GIF** dialog, then create.

> Recommendation: keep the GIF segment **under 30 seconds**, and target **≤ 1 MB** file size.

### 3.2 Optimization presets (Create GIF dialog)

- **Screen Video (Default)** — best for screen recordings.
- **High Motion Video** — for real-world footage or games.
- **Reduced File Size** — scales video down to a **max width/height of 200 px**.
- **Custom** — manual control (see below).

### 3.3 Custom settings

| Setting | Effect |
|---|---|
| **Frame Rate** | Higher = smoother but larger file. |
| **Dimensions** (width/height, or 100% scale) | Lower dimensions = smaller file. |
| **Aspect Ratio Lock** | Recommended to avoid distortion. |
| **Dithering** | Reduces color banding; increases file size. |
| **Looping** | Enable for continuous replay or disable for a single playthrough. Snagit also supports looping a **specific number of times** vs infinite. |
| **Dynamic Colors** (Mac) | Increases file size; recommended for most videos. |
| **Fade to Black** | Adds a transition on the final frame. |

- Multiple video clips can be combined and exported together as a single GIF.
- If a GIF looks blurry, recreate with Custom + higher frame rate (at the cost of larger file size).

---

## 4. Video from Images (screenshots → video / screencast)

Records **voice narration over one or more still images**, with live annotation — a quick way to turn screenshots into a how-to video without a full NLE.

### 4.1 UX flow

1. Capture all the screenshots/images you need.
2. In **Snagit Editor**, select the images in the **Recent Captures tray** or **Library**:
   - `Control+click` (Windows) / `Command+click` (Mac) to multi-select.
3. Click **Create > Video from Images** (a.k.a. "Video from Image").
4. A **recording toolbar** appears. Reorder images by **dragging** them in the tray.
5. Configure settings in the toolbar:
   - **Background Color** (dark/light, for contrast).
   - **Cursor** capture toggle.
   - **Microphone Audio** — enable to narrate.
   - **Webcam** — optionally record camera at the same time.
6. Press **Record** (`Shift+F9` Win / `Control+Shift+Space` Mac) and narrate.
7. **Advance between images** with the **Left/Right arrow keys** or **Next/Previous** buttons in the toolbar.
8. **Annotate while recording** (or while paused): add arrows, shapes, step numbers, etc. from the **Favorites** tool; click-drag to place them on the current image.
9. **Pause/resume** as needed (same hotkey).
10. **Stop** (`Shift+F10` Win / `Control+Shift+V` Mac). The video opens on the canvas for trimming, editing, and sharing as **MP4**.

### 4.2 The other two TechSmith methods (for context)

- **Camtasia Online** — web-based; record up to **5 min of narration per image**, with/without camera; trim, reorder scenes, templates, collaboration; export MP4 or open in Camtasia Editor.
- **Camtasia Editor** (desktop) — combine images with screen/camera/mic recordings on **separate editable tracks**; captions, animations, audio/text-based editing, LMS export.

---

## 5. Picture-in-Picture / Webcam Overlay Editing

- PiP is configured **at capture time** (placement, shape, size, mode) — see §1.6.
- During recording you can **toggle between PiP and Fullscreen Webcam** (`Control+Shift+F`) and **resize/reposition** the webcam preview.
- **Post-recording editing of the PiP overlay** (moving/resizing the webcam within the saved video) is **not clearly documented as a Snagit Editor feature** — Snagit's editor is trim/cut/combine oriented. For repositioning a camera track after the fact, TechSmith points to **Camtasia** (camera on a separate track). **(uncertain — treat PiP as a capture-time decision in Snagit.)**

---

## 6. Audio Editing

- **Recording audio:** system audio + microphone narration captured at record time (§1.4); narration also added via **Video from Images** (§4).
- **Remove audio (Mac only):** **Video menu > Make a Silent Copy** creates an audio-free duplicate while preserving the original.
- **Windows:** no documented in-editor "remove audio" command (full feature parity otherwise). **(uncertain)** whether newer Windows builds add this.
- Snagit's editor does **not** offer multitrack audio mixing, separate audio import, or waveform editing — that's a **Camtasia** capability. Snagit audio is essentially: record it, or (Mac) strip it.

---

## 7. Sharing & Output

### 7.1 Output formats

- **MP4** — primary video export (save to local/network drive).
- **Animated GIF** — via the GIF export (§3).
- Editor also imports/reads **MOV**.

### 7.2 Sharing destinations

- **File** — save MP4 locally or to a network drive.
- **Screencast** (TechSmith Screencast / Screencast.com) — hosted video with shareable link, **conversations/comments and reactions**.
- **Cloud / hosting:** Google Drive, Dropbox, YouTube, Slack, and other configured outputs.
- Snagit **auto-copies the share link to the clipboard** after upload.

### 7.3 Resolution / quality settings

- For **video recording**, output resolution is effectively set by the **captured region dimensions** (the region you draw / the dimension presets). Snagit does **not** expose a separate bitrate/quality dropdown for MP4 in the documented UI. **(uncertain — no official bitrate/CRF control found.)**
- For **GIF**, quality/size is controlled via the **Create GIF** optimization presets and Custom settings (frame rate, dimensions, dithering) — §3.
- No documented frame-rate selector for MP4 recording in official tutorials. **(uncertain.)**

---

## 8. Windows vs Mac — Differences Summary

| Area | Windows | Mac |
|---|---|---|
| Capture hotkey | `Print Screen` | `Control+Shift+C` |
| Record / Pause | `Shift+F9` | `Control+Shift+Space` (some docs: `Command+Shift+Space` for record) |
| Stop | `Shift+F10` | `Control+Shift+V` |
| Aspect lock while dragging | `Control+Shift` (16:9; 4:3 also referenced) | `Shift` (16:9) |
| Webcam toggle label | "Record Webcam" | "Camera" |
| Camera: Mirror | Not documented | Yes |
| Camera: Border w/ color | Not documented | Yes |
| Camera: Virtual background / blur | Not documented | Yes (Intel Macs: blur/none only) |
| Remove audio | Not documented | **Video > Make a Silent Copy** |
| GIF "Dynamic Colors" option | n/a | Yes |
| Playback speed (preview only) | Not documented | Yes (doesn't affect exports) |

PiP switch (`Control+Shift+F`), Save Frame as Image, Cut Out, Add Videos, GIF export, and Video from Images are present on **both** platforms.

---

## 9. Open Questions / Uncertainties

- Exact **countdown** duration before recording and whether it's configurable.
- Hard **maximum recording length** (only a "keep under ~1 hour" recommendation is documented).
- Whether MP4 export exposes any **bitrate / quality / frame-rate** controls (none found in official tutorials).
- Whether **Windows** has an in-editor "remove audio" equivalent in current builds.
- Whether the **webcam/PiP overlay can be repositioned in the Editor after recording** (appears to be capture-time only in Snagit; Camtasia for post editing).
- Mac record hotkey discrepancy (`Control+Shift+Space` vs `Command+Shift+Space`) across docs — likely version-dependent.

---

## Sources

- Record a Video with Snagit — https://www.techsmith.com/learn/tutorials/snagit/how-to-capture-video/
- Record, Edit & Share a Snagit Video — https://www.techsmith.com/learn/tutorials/snagit/record-edit-share-snagit-video/
- Record a Picture-in-Picture Video — https://www.techsmith.com/learn/tutorials/snagit/picture-in-picture/
- Edit a Video — https://www.techsmith.com/learn/tutorials/snagit/editing-video/
- Create an Animated GIF — https://www.techsmith.com/learn/tutorials/snagit/animated-gif/
- 3 Ways to Turn Your Snagit Screenshots into a Video (Video from Images) — https://www.techsmith.com/learn/tutorials/snagit/video-from-images/
- Combine Videos — https://www.techsmith.com/learn/tutorials/snagit/combine-videos/
- Video Screenshot (Save Frame as Image) — https://www.techsmith.com/snagit/features/video-screenshot/
- Video to GIF feature — https://www.techsmith.com/snagit/features/video-to-gif/
- Quick Share Screen Recorder feature — https://www.techsmith.com/snagit/features/quick-share-screen-recorder/
- Snagit Features overview — https://www.techsmith.com/snagit/features/
- Record a Microphone and Incoming Audio from a Webinar or Zoom — https://support.techsmith.com/hc/en-us/articles/360054856751-Record-a-Microphone-and-Incoming-Audio-from-a-Webinar-or-Zoom-With-Snagit
- Snagit Mac Version History — https://support.techsmith.com/hc/en-us/articles/115006443147-Snagit-Mac-Version-History
- Screen capture / screen recorder product page — https://www.techsmith.com/screen-capture.html
