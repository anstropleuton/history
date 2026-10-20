import argparse
import os
import re
import unicodedata
from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional, Sequence, Tuple


@dataclass
class ExistingFileData:
    data_has_errors: bool
    file_path: str
    rays_example: bool
    stars: str
    year: str
    year_start: str
    year_end: str
    name: str
    username: str
    copyright_name: str
    copyright_username: str
    ray_copyrighted: bool
    contributor_name: str
    contributor_username: str
    ray_reviewed: bool
    username_has_no_at: bool
    created_version: str
    updated_version: str


@dataclass
class ReadmeFileData:
    data_has_errors: bool
    file_path: str
    line_number: int
    line: str
    stars: str
    created_version: str
    updated_version: str
    name: str
    user_url: str
    username: str


@dataclass
class WebsiteFileData:
    data_has_errors: bool
    file_path: str
    line_number: int
    line: str
    stars: str


@dataclass
class Issue:
    source: str
    file_path: str
    problem: str
    details: str
    suggestion: str
    line_number: Optional[int] = None
    line: Optional[str] = None

    def sort_key(self) -> tuple:
        return (self.file_path, self.source, self.problem, self.line_number or 0)


RE_STARS = re.compile(r'Example complexity rating: \[([★☆]+)\]', re.DOTALL)
RE_YEAR = re.compile(r'Copyright \(c\) ([\d-]+)', re.DOTALL)
RE_YEAR_START = re.compile(r'Copyright \(c\) (\d+)', re.DOTALL)
RE_YEAR_END = re.compile(r'Copyright \(c\) \d+-(\d+)', re.DOTALL)
RE_COPYRIGHT_NAME = re.compile(r'Copyright \(c\) [\d-]+ (.+?) \(', re.DOTALL)
RE_COPYRIGHT_USERNAME = re.compile(r'Copyright \(c\) [\d-]+ .+? \((.+?)\)', re.DOTALL)
RE_RAY_COPYRIGHTED = re.compile(r'Copyright \(c\) [\d-]+ .+? \(.+?\) and Ramon Santamaria \(@raysan5\)', re.DOTALL)
RE_CONTRIBUTOR_NAME = re.compile(r'Example contributed by (.+?) \(', re.DOTALL)
RE_CONTRIBUTOR_USERNAME = re.compile(r'Example contributed by .+? \((.+?)\)', re.DOTALL)
RE_RAY_REVIEWED = re.compile(r'Example contributed by .+? \(.+?\) and.*reviewed by.*Ramon Santamaria \(@raysan5\)', re.DOTALL)
RE_CREATED_VERSION = re.compile(r'Example originally created with raylib (.+?)[,\n]', re.DOTALL)
RE_UPDATED_VERSION = re.compile(r'Example originally created with raylib .+?, last time updated with raylib (.+?)\n\*', re.DOTALL)

RE_README_FILE_PATH = re.compile(r'^\| *\d+ *\| *\[.+?\]\((.+?)\)')
RE_README_STARS = re.compile(r'width="80"> *\| *([⭐️☆]+?) *\|')
RE_README_CREATED_VERSION = re.compile(r'[⭐️☆]+? *\| *\**([\d.]+?)\** *\|')
RE_README_UPDATED_VERSION = re.compile(r'[⭐️☆]+? *\| *\**[\d.]+?\** *\| *\**([\d.]+?)\** *\|')
RE_README_NAME = re.compile(r'[⭐️☆]+? *\| *\**[\d.]+?\** *\| *\**[\d.]+?\** *\| *\[(.+?)\]')
RE_README_USER_URL = re.compile(r'[⭐️☆]+? *\| *\**[\d.]+?\** *\| *\**[\d.]+?\** *\| *\[.+?\]\((.+?)\)')

RE_WEBSITE_STARS = re.compile(r"  +exampleEntry\('([⭐️☆]+)'")
RE_WEBSITE_MODULE = re.compile(r"  +exampleEntry\('[⭐️☆]+' *, *'(.+?)'")
RE_WEBSITE_NAME = re.compile(r"  +exampleEntry\('[⭐️☆]+' *, *'[^\']+' *, *'(.+?)'")


def get_value_or_default(value, default, flag):
    if value:
        return value.group(1), flag
    return default, True


def replace_stars(stars: str) -> str:
    stars = "".join(c for c in stars if unicodedata.category(c) != "Mn")
    return "".join("#" if star in ("⭐", "★") else "_" if star == "☆" else star for star in stars)


def norm(s: str) -> str:
    return re.sub(r"\s+", " ", s.strip())


def print_table(headers: Sequence[str], rows: Sequence[Tuple[Sequence[str], Sequence[Sequence[str]]]]) -> None:
    widths = [
        max(len(str(header)), max((len(str(row_data[i])) for row_data, _ in rows), default=0))
        for i, header in enumerate(headers)
    ]

    def format_row(values: Sequence[str]) -> str:
        return "| " + " | ".join(str(value).ljust(widths[i]) for i, value in enumerate(values)) + " |"

    separator_row = "| " + " | ".join("=" * width for width in widths) + " |"
    top_border = "+" + "+".join("-" * (width + 2) for width in widths) + "+"

    print(top_border)
    print(format_row(headers))
    print(separator_row)
    for row_data, sub_rows in rows:
        print(format_row(row_data))
        for sub_row in sub_rows:
            print("  " + format_row(sub_row))
    print(top_border)


def extract_existing_file_data(file_path: str) -> ExistingFileData:
    content = Path(file_path).read_text(encoding="utf-8")

    stars = re.search(RE_STARS, content)
    year = re.search(RE_YEAR, content)
    year_start = re.search(RE_YEAR_START, content)
    year_end = re.search(RE_YEAR_END, content)
    copyright_name = re.search(RE_COPYRIGHT_NAME, content)
    copyright_username = re.search(RE_COPYRIGHT_USERNAME, content)
    ray_copyrighted = bool(RE_RAY_COPYRIGHTED.search(content))
    contributor_name = re.search(RE_CONTRIBUTOR_NAME, content)
    contributor_username = re.search(RE_CONTRIBUTOR_USERNAME, content)
    ray_reviewed = bool(RE_RAY_REVIEWED.search(content))
    created_version = re.search(RE_CREATED_VERSION, content)
    updated_version = re.search(RE_UPDATED_VERSION, content)

    data_has_errors = False
    stars, data_has_errors = get_value_or_default(stars, "-", data_has_errors)
    year, data_has_errors = get_value_or_default(year, "-", data_has_errors)
    year_start, data_has_errors = get_value_or_default(year_start, "-", data_has_errors)
    year_end, data_has_errors = get_value_or_default(year_end, "-", data_has_errors)
    copyright_name, _ = get_value_or_default(copyright_name, "-", data_has_errors)
    copyright_username, _ = get_value_or_default(copyright_username, "-", data_has_errors)
    contributor_name, _ = get_value_or_default(contributor_name, "-", data_has_errors)
    contributor_username, _ = get_value_or_default(contributor_username, "-", data_has_errors)
    created_version, data_has_errors = get_value_or_default(created_version, "-", data_has_errors)
    updated_version, data_has_errors = get_value_or_default(updated_version, "-", data_has_errors)

    if copyright_name.endswith(" and Ramon Santamaria"):
        copyright_name = copyright_name.replace(" and Ramon Santamaria", "")
        copyright_username = "-"
        data_has_errors = True

    if contributor_name.endswith(" and reviewed by Ramon Santamaria"):
        contributor_name = contributor_name.replace(" and reviewed by Ramon Santamaria", "")
        contributor_username = "-"
        data_has_errors = True

    rays_example = copyright_name == "Ramon Santamaria"
    if rays_example:
        name = "Ray"
        ray_copyrighted = True
        ray_reviewed = True
    else:
        name = copyright_name if copyright_name == contributor_name else "-"
        copyright_username = copyright_username if copyright_username == contributor_username else "-"
        data_has_errors |= not ray_reviewed

    username = copyright_username
    username_has_no_at = not username.startswith("@")
    data_has_errors |= username_has_no_at

    stars = replace_stars(str(stars))

    return ExistingFileData(
        data_has_errors=data_has_errors,
        file_path=file_path,
        rays_example=rays_example,
        stars=stars,
        year=year,
        year_start=year_start,
        year_end=year_end,
        name=name,
        username=username,
        copyright_name=copyright_name,
        copyright_username=copyright_username,
        ray_copyrighted=ray_copyrighted,
        contributor_name=contributor_name,
        contributor_username=contributor_username,
        ray_reviewed=ray_reviewed,
        username_has_no_at=username_has_no_at,
        created_version=created_version,
        updated_version=updated_version,
    )


def extract_readme_file_data(line_number: int, line: str) -> ReadmeFileData:
    file_path = re.search(RE_README_FILE_PATH, line)
    stars = re.search(RE_README_STARS, line)
    created_version = re.search(RE_README_CREATED_VERSION, line)
    updated_version = re.search(RE_README_UPDATED_VERSION, line)
    name = re.search(RE_README_NAME, line)
    user_url = re.search(RE_README_USER_URL, line)

    data_has_errors = False
    file_path, data_has_errors = get_value_or_default(file_path, "-", data_has_errors)
    stars, data_has_errors = get_value_or_default(stars, "-", data_has_errors)
    created_version, data_has_errors = get_value_or_default(created_version, "-", data_has_errors)
    updated_version, data_has_errors = get_value_or_default(updated_version, "-", data_has_errors)
    name, data_has_errors = get_value_or_default(name, "-", data_has_errors)
    user_url, data_has_errors = get_value_or_default(user_url, "-", data_has_errors)

    stars = replace_stars(str(stars))

    username = "-"
    if user_url.startswith("https://github.com/"):
        username = user_url.replace("https://github.com/", "@")

    return ReadmeFileData(
        data_has_errors=data_has_errors,
        file_path=file_path,
        line_number=line_number,
        line=line,
        stars=stars,
        created_version=created_version,
        updated_version=updated_version,
        name=name,
        user_url=user_url,
        username=username,
    )


def extract_website_file_data(line_number: int, line: str) -> WebsiteFileData:
    match_stars = re.search(RE_WEBSITE_STARS, line)
    match_module = re.search(RE_WEBSITE_MODULE, line)
    match_name = re.search(RE_WEBSITE_NAME, line)

    data_has_errors = False
    stars, data_has_errors = get_value_or_default(match_stars, "-", data_has_errors)
    module, data_has_errors = get_value_or_default(match_module, "-", data_has_errors)
    name, data_has_errors = get_value_or_default(match_name, "-", data_has_errors)

    stars = replace_stars(stars)
    file_path = f"{module}/{module}_{name}.c"

    return WebsiteFileData(
        data_has_errors=data_has_errors,
        file_path=file_path,
        line_number=line_number,
        line=line,
        stars=stars,
    )


def collect_existing_files() -> List[str]:
    existing_files = []
    for root, _, files in os.walk("."):
        for file in files:
            if file.endswith(".c"):
                file_path = os.path.join(root, file)[2:]
                if file_path != "examples_template.c":
                    existing_files.append(file_path)
    return existing_files


def process_existing_files() -> List[ExistingFileData]:
    return [extract_existing_file_data(file_path) for file_path in collect_existing_files()]


def process_readme() -> List[ReadmeFileData]:
    readme_files_data = []
    with Path("./README.md").open("r", encoding="utf-8") as readme:
        for line_number, line in enumerate(readme, start=1):
            line = line.rstrip("\n")
            if re.match(r"^\| \d+ \|", line):
                readme_files_data.append(extract_readme_file_data(line_number, line))
    return readme_files_data


def process_examples_js() -> List[WebsiteFileData]:
    website_files_data = []
    with Path("../../raylib.com/common/examples.js").open("r", encoding="utf-8") as js_file:
        for line_number, line in enumerate(js_file, start=1):
            line = line.rstrip("\n")
            if re.search(r"  +exampleEntry\('", line):
                website_files_data.append(extract_website_file_data(line_number, line))
    return website_files_data


def make_issue(
    source: str,
    file_path: str,
    problem: str,
    details: str,
    suggestion: str,
    line_number: Optional[int] = None,
    line: Optional[str] = None,
) -> Issue:
    return Issue(source, file_path, problem, details, suggestion, line_number, line)


def suggest_readme_line_from_existing(existing: ExistingFileData) -> str:
    username = existing.username if existing.username.startswith("@") else f"@{existing.username.lstrip('@')}"
    filename = Path(existing.file_path).name
    return (
        f"| ? | [{filename}]({existing.file_path}) | "
        f"{existing.stars} | {existing.created_version} | {existing.updated_version} | "
        f"[{existing.name}](https://github.com/{username.lstrip('@')}) |"
    )


def suggest_website_entry_from_readme(readme: ReadmeFileData) -> str:
    module = Path(readme.file_path).parent.name
    stem = Path(readme.file_path).stem
    if stem.startswith(module + "_"):
        stem = stem[len(module) + 1 :]
    return f"exampleEntry('{readme.stars}', '{module}', '{stem}')"


def analyze_existing_file(existing: ExistingFileData) -> List[Issue]:
    issues: List[Issue] = []

    if existing.name == "-":
        issues.append(make_issue(
            "existing", existing.file_path,
            "unresolved author name",
            "The copyright/contributor names do not match or were not parsed cleanly.",
            "Make the copyright and contributor names identical unless this example is intentionally different."
        ))

    if existing.username_has_no_at:
        issues.append(make_issue(
            "existing", existing.file_path,
            "username formatting",
            "The username does not start with '@'.",
            "Change the username to the form '@handle'."
        ))

    if not existing.ray_reviewed and not existing.rays_example:
        issues.append(make_issue(
            "existing", existing.file_path,
            "missing Ray review",
            "This example is not marked as reviewed by Ramon Santamaria.",
            "Add 'and reviewed by Ramon Santamaria (@raysan5)' to the example header if that review is intended."
        ))

    if existing.created_version == "-" or existing.updated_version == "-":
        issues.append(make_issue(
            "existing", existing.file_path,
            "missing version metadata",
            "The raylib version metadata is incomplete.",
            "Fill in both 'originally created with raylib X.Y' and 'last time updated with raylib X.Y'."
        ))

    if existing.copyright_name == "-" or existing.contributor_name == "-":
        issues.append(make_issue(
            "existing", existing.file_path,
            "missing contributor fields",
            "The contributor/copyright fields could not be parsed cleanly.",
            "Check the header formatting around the copyright and contribution lines."
        ))

    return issues


def compare_readme_to_existing(readme: ReadmeFileData, existing: ExistingFileData) -> List[Issue]:
    issues: List[Issue] = []

    if readme.stars != "-" and existing.stars != "-" and readme.stars != existing.stars:
        issues.append(make_issue(
            "cross-check", readme.file_path,
            "star mismatch",
            f"README stars are {readme.stars}, but the source file says {existing.stars}.",
            f"Update the README stars for this example to {existing.stars}.",
            readme.line_number, readme.line
        ))

    if readme.created_version != "-" and existing.created_version != "-" and readme.created_version != existing.created_version:
        issues.append(make_issue(
            "cross-check", readme.file_path,
            "created version mismatch",
            f"README says {readme.created_version}, but the source file says {existing.created_version}.",
            f"Update the README 'created version' to {existing.created_version}.",
            readme.line_number, readme.line
        ))

    if readme.updated_version != "-" and existing.updated_version != "-" and readme.updated_version != existing.updated_version:
        issues.append(make_issue(
            "cross-check", readme.file_path,
            "updated version mismatch",
            f"README says {readme.updated_version}, but the source file says {existing.updated_version}.",
            f"Update the README 'updated version' to {existing.updated_version}.",
            readme.line_number, readme.line
        ))

    if readme.name != "-" and existing.name != "-" and norm(readme.name) != norm(existing.name):
        issues.append(make_issue(
            "cross-check", readme.file_path,
            "name mismatch",
            f"README name is '{readme.name}', but the source file name is '{existing.name}'.",
            f"Update the README contributor name to '{existing.name}'.",
            readme.line_number, readme.line
        ))

    if readme.username != "-" and existing.username != "-" and norm(readme.username) != norm(existing.username):
        issues.append(make_issue(
            "cross-check", readme.file_path,
            "username mismatch",
            f"README username is '{readme.username}', but the source file username is '{existing.username}'.",
            f"Update the README username to '{existing.username}'.",
            readme.line_number, readme.line
        ))

    return issues


def compare_website_to_readme(website: WebsiteFileData, readme: ReadmeFileData) -> List[Issue]:
    issues: List[Issue] = []

    if website.stars != "-" and readme.stars != "-" and website.stars != readme.stars:
        issues.append(make_issue(
            "cross-check", website.file_path,
            "website star mismatch",
            f"Website stars are {website.stars}, but README stars are {readme.stars}.",
            f"Update examples.js to use '{readme.stars}' for this example.",
            website.line_number, website.line
        ))

    return issues


def file_presence_issues(existing_paths: set[str], readme_files: List[ReadmeFileData], website_files: List[WebsiteFileData]) -> List[Issue]:
    issues: List[Issue] = []

    for readme in readme_files:
        if readme.file_path not in existing_paths:
            issues.append(make_issue(
                "README", readme.file_path,
                "file missing on disk",
                "README lists this example, but the file does not exist.",
                "Remove the README row or restore the missing .c file at the listed path.",
                readme.line_number, readme.line
            ))

    for website in website_files:
        if website.file_path not in existing_paths:
            issues.append(make_issue(
                "website", website.file_path,
                "file missing on disk",
                "examples.js lists this example, but the file does not exist.",
                "Remove the website entry or restore the missing .c file at the listed path.",
                website.line_number, website.line
            ))

    return issues


def summarize_row_issues(file_path: str, issues: List[Issue]) -> str:
    labels = [i.problem for i in issues if i.file_path == file_path]
    return "; ".join(labels) if labels else "-"


def build_issues(existing_files_data, readme_files_data, website_files_data) -> List[Issue]:
    existing_by_path = {e.file_path: e for e in existing_files_data}
    readme_by_path = {r.file_path: r for r in readme_files_data}

    issues: List[Issue] = []
    for existing in existing_files_data:
        issues.extend(analyze_existing_file(existing))
    issues.extend(file_presence_issues(set(existing_by_path), readme_files_data, website_files_data))
    for readme in readme_files_data:
        existing = existing_by_path.get(readme.file_path)
        if existing:
            issues.extend(compare_readme_to_existing(readme, existing))
    for website in website_files_data:
        readme = readme_by_path.get(website.file_path)
        if readme:
            issues.extend(compare_website_to_readme(website, readme))
    return issues


def filter_issues(
    issues: List[Issue],
    *,
    file_pattern: Optional[str] = None,
    source: Optional[set[str]] = None,
    problem_pattern: Optional[str] = None,
) -> List[Issue]:
    file_re = re.compile(file_pattern) if file_pattern else None
    problem_re = re.compile(problem_pattern) if problem_pattern else None

    out: List[Issue] = []
    for i in issues:
        if source and i.source not in source:
            continue
        if file_re and not file_re.search(i.file_path):
            continue
        if problem_re and not problem_re.search(i.problem):
            continue
        out.append(i)
    return out


def sort_issues(issues: List[Issue], sort_mode: str) -> List[Issue]:
    source_order = {"existing": 0, "cross-check": 1, "README": 2, "website": 3}

    if sort_mode == "file":
        return sorted(issues, key=lambda i: (i.file_path, i.problem))
    if sort_mode == "problem":
        return sorted(issues, key=lambda i: (i.problem, i.file_path))
    if sort_mode == "source":
        return sorted(issues, key=lambda i: (source_order.get(i.source, 9), i.file_path, i.problem))
    return issues


def print_inventory_tables(
    existing_files_data: List[ExistingFileData],
    readme_files_data: List[ReadmeFileData],
    website_files_data: List[WebsiteFileData],
    *,
    only_errors: bool = False,
) -> None:
    print("## === === === EXISTING EXAMPLES FILES === === ===")
    existing_headers = ["?", "File", "Stars", "Year", "Created", "Updated", "Name", "Username", "Problems"]
    existing_rows = []
    for item in existing_files_data:
        if only_errors and not item.data_has_errors:
            continue
        issues = analyze_existing_file(item)
        existing_rows.append((
            [
                "×" if item.data_has_errors else "√",
                item.file_path,
                item.stars,
                item.year,
                item.created_version,
                item.updated_version,
                item.name,
                item.username,
                summarize_row_issues(item.file_path, issues) if issues else "-",
            ],
            []
        ))
    print_table(existing_headers, existing_rows)
    print()

    print("## === === === README.md TABLE === === ===")
    readme_headers = ["?", "File", "Stars", "Created", "Updated", "Name", "Username", "URL", "Problems"]
    readme_rows = []
    for item in readme_files_data:
        if only_errors and not item.data_has_errors:
            continue
        readme_rows.append((
            [
                "×" if item.data_has_errors else "√",
                item.file_path,
                item.stars,
                item.created_version,
                item.updated_version,
                item.name,
                item.username,
                item.user_url,
                "parse error" if item.data_has_errors else "-",
            ],
            [[f"Line {item.line_number}", item.line]] if item.data_has_errors else []
        ))
    print_table(readme_headers, readme_rows)
    print()

    print("## === === === examples.js TABLE === === ===")
    website_headers = ["?", "File", "Stars", "Problems"]
    website_rows = []
    for item in website_files_data:
        if only_errors and not item.data_has_errors:
            continue
        website_rows.append((
            [
                "×" if item.data_has_errors else "√",
                item.file_path,
                item.stars,
                "parse error" if item.data_has_errors else "-",
            ],
            [[f"Line {item.line_number}", item.line]] if item.data_has_errors else []
        ))
    print_table(website_headers, website_rows)
    print()


def print_issue_report(issues: List[Issue]) -> None:
    headers = ["#", "Source", "File", "Problem", "Suggestion"]
    rows = []
    for idx, i in enumerate(issues, start=1):
        sub = []
        rows.append((
            [str(idx), i.source, i.file_path, i.problem, i.suggestion],
            sub
        ))

    print("## === === === SORTED PROBLEMS === === ===")
    print_table(headers, rows)
    print()

    grouped: dict[str, List[Issue]] = {}
    for i in issues:
        grouped.setdefault(i.file_path, []).append(i)


def main() -> None:
    parser = argparse.ArgumentParser(description="raylib examples consistency checker")
    parser.add_argument("--only-errors", action="store_true", help="show only rows with parse/validation errors in source tables")
    parser.add_argument("--source", nargs="*", default=None, choices=["existing", "readme", "website", "cross-check"], help="filter issues by source")
    parser.add_argument("--file", dest="file_pattern", default=None, help="regex filter on file path")
    parser.add_argument("--problem", dest="problem_pattern", default=None, help="regex filter on problem label")
    parser.add_argument("--sort", default="problem", choices=["file", "problem", "source"], help="issue sort mode")
    args = parser.parse_args()

    current_dir = os.getcwd()
    if not current_dir.endswith("raylib/examples"):
        print("Incorrect directory. Please run the script in raylib/examples")
        print("Also, have raylib.com repository cloned in the same directory as raylib")
        return

    if not os.path.exists(current_dir.replace("raylib/examples", "raylib.com/common")):
        print(f"raylib.com directory does not exist in parent {current_dir.replace('raylib/examples', '')}.")
        print("Please have raylib.com repository cloned in the same directory as raylib")
        return

    existing_files_data = process_existing_files()
    readme_files_data = process_readme()
    website_files_data = process_examples_js()

    print("# === === === PROCESSING FILES === === ===")
    print("Unchecked checkbox means it contains errors")
    print("Please review even checked checkbox")
    print("Stars are displayed using # (for full) and _ (for empty) rather than ⭐️, ★ or ☆")
    print()

    print_inventory_tables(
        existing_files_data,
        readme_files_data,
        website_files_data,
        only_errors=args.only_errors,
    )

    issues = build_issues(existing_files_data, readme_files_data, website_files_data)

    source_filter = set(args.source) if args.source else None
    issues = filter_issues(
        issues,
        file_pattern=args.file_pattern,
        source=source_filter,
        problem_pattern=args.problem_pattern,
    )
    issues = sort_issues(issues, args.sort)

    print_issue_report(issues)


if __name__ == "__main__":
    main()