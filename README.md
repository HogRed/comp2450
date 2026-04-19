# COMP 2450 — The Descent

Data Structures & Algorithms in C++, themed as a dungeon crawl. Harding University.

This repo is both the **course website** (served via GitHub Pages) and the source of truth for all **student-facing** teaching materials.

## Layout

```
comp2450/
├── index.md                  student-facing homepage (the Dungeon Entrance)
├── syllabus.md               student syllabus
├── _config.yml               Jekyll / GitHub Pages config
├── _layouts/                 page templates (HTML)
├── assets/css/               visual identity (Diablo-style stylesheet)
├── _floors/                  one page per week — what students see
│   └── floor-00.md           Floor 0: The Antechamber (Week 1)
└── project/                  the semester-long dungeon-crawler project
    └── floor-00-starter/     starter code handed out at each floor
```

Instructor-only prep (session plans, answer keys, video scripts, grading notes) is kept **locally** — outside version control — and never committed to this public repo.

## Viewing the site

- **Live site:** https://hogred.github.io/comp2450/
- **On GitHub:** markdown files render fine on github.com without any Jekyll build.
- **Locally:** full Jekyll preview on Windows requires Ruby — usually not worth the setup; just push and check the deployed site.

## Editing

Edit any student-facing page freely. Students see the latest version after each push; GitHub Pages rebuilds in 30–90 seconds.
