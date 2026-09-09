# Setting Up on GitHub

## Option 1: Create a New Repository (Recommended)

### Step 1: Create Repository on GitHub

1. Go to https://github.com/new
2. Fill in the repository details:
   - **Repository name**: `diff-tool` (or your preferred name)
   - **Description**: "A high-performance diff tool in C++ implementing Myers' algorithm"
   - **Public/Private**: Choose as desired
   - **Initialize README**: ❌ No (we have one)
   - **Add .gitignore**: ❌ No (we have one)
   - **Add license**: ❌ No (we have MIT)
3. Click "Create repository"

### Step 2: Connect Local Repository to GitHub

In your project directory:

```bash
# Add the remote repository
git remote add origin https://github.com/YOUR_USERNAME/diff-tool.git

# Rename branch if needed (GitHub default is 'main')
git branch -m master main

# Push to GitHub
git push -u origin main
```

### Step 3: Verify

Visit `https://github.com/YOUR_USERNAME/diff-tool` and confirm all files are there.

---

## Option 2: Using SSH (More Secure)

If you have SSH keys configured:

```bash
# Add SSH remote instead
git remote add origin git@github.com:YOUR_USERNAME/diff-tool.git

# Push to GitHub
git push -u origin main
```

---

## Option 3: GitHub CLI (Easiest)

If you have GitHub CLI installed:

```bash
# Create and push in one command
gh repo create diff-tool --source=. --push --public

# Or for private repo:
gh repo create diff-tool --source=. --push --private
```

---

## Next Steps After Pushing

### Add GitHub-Specific Files

#### 1. Create `CONTRIBUTING.md`

```markdown
# Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Development Setup

```bash
make clean
make debug
make test
```

## Code Style

- Follow Google C++ style guide
- Use 4-space indentation
- Add tests for new features
- Update documentation
```

#### 2. Create GitHub Actions Workflow

Create `.github/workflows/build.yml`:

```yaml
name: Build and Test

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install dependencies
        run: sudo apt-get install -y build-essential cmake
      - name: Build
        run: |
          mkdir build
          cd build
          cmake ..
          make
      - name: Run tests
        run: cd build && ctest --verbose
```

#### 3. Create GitHub Issues Template

Create `.github/ISSUE_TEMPLATE/bug_report.md`:

```markdown
---
name: Bug report
about: Report a bug
---

## Description
<!-- Brief description of the bug -->

## Steps to Reproduce
1. ...
2. ...

## Expected Behavior
<!-- What should happen -->

## Actual Behavior
<!-- What actually happens -->

## Environment
- OS: 
- Compiler: 
- Version: 

## Error Output
<!-- Error messages or logs -->
```

---

## Creating Releases

### Step 1: Tag a Release

```bash
# Create an annotated tag
git tag -a v1.0.0 -m "Release version 1.0.0"

# Push tags to GitHub
git push origin v1.0.0
```

### Step 2: Create Release on GitHub

1. Go to your repository
2. Click "Releases" → "Create a new release"
3. Select tag `v1.0.0`
4. Title: "v1.0.0 - Initial Release"
5. Description:

```markdown
## Features
- ✅ Myers' diff algorithm
- ✅ Multiple output formats
- ✅ Streaming file I/O
- ✅ Binary file detection
- ✅ Comprehensive tests

## Installation
```bash
cmake .
make install
```

## Usage
```bash
./diff file1.txt file2.txt
```

See [README.md](https://github.com/YOUR_USERNAME/diff-tool/blob/main/README.md) for details.
```

6. Publish release

---

## Adding Topics & Description

Go to your repository settings and add:

**Topics**: `cpp`, `algorithms`, `diff`, `systems-programming`, `tools`

**Repository name**: `diff-tool`

**Description**: "A high-performance diff tool in C++ - demonstrates Myers' algorithm, systems programming, and file I/O optimization"

---

## Statistics Badge (Optional)

Add to README.md for visual appeal:

```markdown
![GitHub stars](https://img.shields.io/github/stars/YOUR_USERNAME/diff-tool)
![GitHub license](https://img.shields.io/github/license/YOUR_USERNAME/diff-tool)
![GitHub language](https://img.shields.io/github/languages/top/YOUR_USERNAME/diff-tool)
```

---

## Protecting Main Branch (Recommended)

1. Go to repository Settings → Branches
2. Click "Add rule" under "Branch protection rules"
3. Pattern: `main`
4. Enable:
   - ✅ Require pull request reviews before merging
   - ✅ Require status checks to pass
   - ✅ Include administrators

This ensures code quality before merging.

---

## Troubleshooting

### "fatal: remote origin already exists"

```bash
git remote remove origin
git remote add origin https://github.com/YOUR_USERNAME/diff-tool.git
```

### "Authentication failed"

Using HTTPS? Create a Personal Access Token:
1. GitHub Settings → Developer settings → Personal access tokens
2. Create token with `repo` scope
3. Use token as password when prompted

Or switch to SSH (more secure for repeated pushes).

### "nothing to commit, working tree clean"

Your commits are already pushed. Verify on GitHub.

---

## Final Checklist

- ✅ Repository created on GitHub
- ✅ Code pushed to main branch
- ✅ README.md visible and formatted
- ✅ All files present
- ✅ GitHub Actions workflow running
- ✅ MIT License displayed
- ✅ Topics added
- ✅ (Optional) Branch protection enabled
- ✅ (Optional) Release created

Your diff-tool is now public and ready for the world! 🚀

---

## Share Your Project

Share on:
- Twitter: "I built a diff tool in C++ implementing Myers' algorithm! Check it out: https://github.com/YOUR_USERNAME/diff-tool"
- Reddit: r/cpp, r/learnprogramming
- Dev.to: Cross-post your blog
- Hacker News: Submit link if significant
- LinkedIn: Showcase in projects section

Good luck! 🎉
