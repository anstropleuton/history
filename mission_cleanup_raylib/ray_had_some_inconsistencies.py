import os
import re
import unicodedata
from dataclasses import dataclass
from typing import List


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


def get_value_or_default(value, default, flag):
    if value:
        return value.group(1), flag
    else:
        return default, True


def replace_stars(stars):
    stars = ''.join(c for c in stars if unicodedata.category(c) != 'Mn')
    return ''.join('#' if star == '⭐' or star == '★' else '_' if star == '☆' else star for star in stars)


# ChatGPT writing good code is a rare occurrence
def print_table(headers, rows):
    column_widths = [
        max(len(str(header)), max((len(str(row_data[i])) for row_data, _ in rows), default=0))
        for i, header in enumerate(headers)
    ]

    def format_row(values):
        return "| " + " | ".join(f"{str(value).ljust(column_widths[i])}" for i, value in enumerate(values)) + " |"

    separator_row = "| " + " | ".join("=" * width for width in column_widths) + " |"
    top_border = "+" + "+".join("-" * (width + 2) for width in column_widths) + "+"

    print(top_border)
    print(format_row(headers))
    print(separator_row)
    for row_data, sub_rows in rows:
        print(format_row(row_data))
        for sub_row in sub_rows:
            print("  " + format_row(sub_row))
    print(top_border)


def extract_existing_file_data(file_path: str) -> ExistingFileData:
    with open(file_path, "r") as f:
        content = f.read()

    stars = re.search(r'Example complexity rating: \[([★☆]+)\]', content, re.DOTALL)
    year = re.search(r'Copyright \(c\) ([\d-]+)', content, re.DOTALL)
    year_start = re.search(r'Copyright \(c\) (\d+)', content, re.DOTALL)
    year_end = re.search(r'Copyright \(c\) \d+-(\d+)', content, re.DOTALL)
    copyright_name = re.search(r'Copyright \(c\) [\d-]+ (.+?) \(', content, re.DOTALL)
    copyright_username = re.search(r'Copyright \(c\) [\d-]+ .+? \((.+?)\)', content, re.DOTALL)
    ray_copyrighted = re.search(r'Copyright \(c\) [\d-]+ .+? \(.+?\) and Ramon Santamaria \(@raysan5\)', content, re.DOTALL)
    contributor_name = re.search(r'Example contributed by (.+?) \(', content, re.DOTALL)
    contributor_username = re.search(r'Example contributed by .+? \((.+?)\)', content, re.DOTALL)

    # "and.*reviewed" is edge case, Ray or contributor wrapped the string
    # "by.*Ramon" is also and edge case because someone else also reviewed an example
    ray_reviewed = re.search(r'Example contributed by .+? \(.+?\) and.*reviewed by.*Ramon Santamaria \(@raysan5\)', content, re.DOTALL)
    created_version = re.search(r'Example originally created with raylib (.+?)[,\n]', content, re.DOTALL)
    updated_version = re.search(r'Example originally created with raylib .+?, last time updated with raylib (.+?)\n\*', content, re.DOTALL)

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

    # Copyright username not found
    if copyright_name.endswith(" and Ramon Santamaria"):
        copyright_name = copyright_name.replace(" and Ramon Santamaria", "")
        copyright_username = "-"
        data_has_errors = True

    # Contributor username not found
    if contributor_name.endswith(" and reviewed by Ramon Santamaria"):
        contributor_name = contributor_name.replace(" and reviewed by Ramon Santamaria", "")
        contributor_username = "-"
        data_has_errors = True

    name = copyright_name
    username = copyright_username

    # Ray's personal example
    rays_example = name == "Ramon Santamaria"
    if rays_example:
        name = "Ray"
        ray_copyrighted = True
        ray_reviewed = True
    else:
        name = copyright_name if copyright_name == contributor_name else "-"
        username = copyright_username if copyright_username == contributor_username else "-"
        # data_has_errors |= not ray_copyrighted
        data_has_errors |= not ray_reviewed

    # Username does not start with @
    username_has_no_at = not username.startswith("@")
    data_has_errors |= username_has_no_at

    stars = replace_stars(str(stars))

    return ExistingFileData(
        data_has_errors=data_has_errors,
        rays_example=rays_example,
        file_path=file_path,
        stars=stars,
        year=year,
        year_start=year_start,
        year_end=year_end,
        name=name,
        username=username,
        copyright_name=copyright_name,
        copyright_username=copyright_username,
        ray_copyrighted=bool(ray_copyrighted),
        contributor_name=contributor_name,
        contributor_username=contributor_username,
        ray_reviewed=bool(ray_reviewed),
        username_has_no_at=username_has_no_at,
        created_version=created_version,
        updated_version=updated_version
    )


def extract_readme_file_data(line_number: int, line: str) -> ReadmeFileData:
    file_path = re.search(r'^\| *\d+ *\| *\[.+?\]\((.+?)\)', line)
    stars = re.search(r'width="80"> *\| *([⭐️☆]+?) *\|', line)
    created_version = re.search(r'[⭐️☆]+? *\| *\**([\d.]+?)\** *\|', line)
    updated_version = re.search(r'[⭐️☆]+? *\| *\**[\d.]+?\** *\| *\**([\d.]+?)\** *\|', line)
    name = re.search(r'[⭐️☆]+? *\| *\**[\d.]+?\** *\| *\**[\d.]+?\** *\| *\[(.+?)\]', line)
    user_url = re.search(r'[⭐️☆]+? *\| *\**[\d.]+?\** *\| *\**[\d.]+?\** *\| *\[.+?\]\((.+?)\)', line)

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
        username=username
    )


def extract_website_file_data(line_number: int, line: str) -> WebsiteFileData:
    match_stars = re.search(r'  +exampleEntry\(\'([⭐️☆]+)\'', line)
    match_module = re.search(r'  +exampleEntry\(\'[⭐️☆]+\' *, *\'(.+?)\'', line)
    match_name = re.search(r'  +exampleEntry\(\'[⭐️☆]+\' *, *\'[^\']+\' *, *\'(.+?)\'', line)

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
        stars=stars
    )


def collect_existing_files() -> List[str]:
    existing_files = []
    for root, _, files in os.walk("."):
        for file in files:
            if file.endswith(".c"):
                file_path = os.path.join(root, file)
                file_path = file_path[2:]
                if file_path != "examples_template.c":
                    existing_files.append(file_path)
    return existing_files


def process_existing_files() -> List[ExistingFileData]:
    existing_files_data = []
    existing_files = collect_existing_files()
    for file_path in existing_files:
        existing_files_data.append(extract_existing_file_data(file_path))
    return existing_files_data


def process_readme() -> List[ReadmeFileData]:
    readme_files_data = []
    readme_file_path = os.path.expanduser('./README.md')
    with open(readme_file_path, "r") as readme:
        for line_number, line in enumerate(readme, start=1):
            line = line[:-1]

            # Only process lines with table entries
            if re.match(r'^\| \d+ \|', line):
                readme_files_data.append(extract_readme_file_data(line_number, line))
    return readme_files_data


def process_examples_js() -> List[WebsiteFileData]:
    website_files_data = []
    js_file_path = os.path.expanduser('../../raylib.com/common/examples.js')
    with open(js_file_path, "r") as js_file:
        for line_number, line in enumerate(js_file, start=1):
            line = line[:-1]

            # Only process lines with the exampleEntry pattern
            if re.search(r'  +exampleEntry\(\'', line):
                website_files_data.append(extract_website_file_data(line_number, line))
    return website_files_data


def print_file_comparison_results(existing_files_data: List[ExistingFileData],
                                  readme_files_data: List[ReadmeFileData],
                                  website_files_data: List[WebsiteFileData]):
    print("# === === === FILE COMPARISON RESULTS === === ===")

    print("Files listed in README.md but not found:")
    for file_data in readme_files_data:
        if file_data.file_path not in [e.file_path for e in existing_files_data]:
            print(f"- {file_data.file_path}")
    print()

    print("Files listed in examples.js but not found:")
    for file_data in website_files_data:
        if file_data.file_path not in [e.file_path for e in existing_files_data]:
            print(f"- {file_data.file_path}")
    print()

    print("Files found but not listed in README.md:")
    for file_data in existing_files_data:
        if file_data.file_path not in [e.file_path for e in readme_files_data]:
            print(f"- {file_data.file_path}")
    print()

    print("Files found but not listed in examples.js:")
    for file_data in existing_files_data:
        if file_data.file_path not in [e.file_path for e in website_files_data]:
            print(f"- {file_data.file_path}")
    print()

    print("Files listed in README.md but not listed in examples.js:")
    for file_data in readme_files_data:
        if file_data.file_path not in [e.file_path for e in website_files_data]:
            print(f"- {file_data.file_path}")
    print()

    print("Files listed in examples.js but not listed in README.md:")
    for file_data in website_files_data:
        if file_data.file_path not in [e.file_path for e in readme_files_data]:
            print(f"- {file_data.file_path}")
    print()


def print_star_comparison_results(readme_files_data: List[ReadmeFileData],
                                  website_files_data: List[WebsiteFileData]):
    print("# === === === STAR COMPARISON RESULTS === === ===")
    print("Stars are displayed using # (for full) and _ (for empty) rather than ⭐️, ★ or ☆")

    print("Stars from README.md (Readme Stars) that does not match stars shown in website (Website Stars, from common/examples.js):")
    stars_difference_headers = ["File", "Readme Stars", "Website Stars"]
    stars_difference_rows = []

    for readme_file_data in readme_files_data:
        for website_file_data in website_files_data:
            if readme_file_data.file_path == website_file_data.file_path:
                if readme_file_data.stars != website_file_data.stars:
                    stars_difference_rows.append((
                        [readme_file_data.file_path, readme_file_data.stars, website_file_data.stars],
                        []
                    ))

    print_table(stars_difference_headers, stars_difference_rows)
    print()


def print_readme_comparison_results(existing_files_data: List[ExistingFileData],
                                    readme_files_data: List[ReadmeFileData]):
    print("# === === === README.md DATA COMPARISON RESULTS === === ===")
    print("Stars are displayed using # (for full) and _ (for empty) rather than ⭐️, ★ or ☆")

    print("README.md file data (Readme Value) that does not match with existing file data (Existing Value):")
    difference_headers = ["File", "Field", "Readme Value", "Existing Value"]
    difference_rows = []

    for readme_file_data in readme_files_data:
        for existing_file_data in existing_files_data:
            if readme_file_data.file_path == existing_file_data.file_path:
                if readme_file_data.stars != "-" and existing_file_data.stars != "-" and readme_file_data.stars != existing_file_data.stars:
                    difference_rows.append(([readme_file_data.file_path, "Stars", readme_file_data.stars, existing_file_data.stars], []))

                if readme_file_data.created_version != "-" and existing_file_data.created_version != "-" and readme_file_data.created_version != existing_file_data.created_version:
                    difference_rows.append(([readme_file_data.file_path, "Created Version", readme_file_data.created_version, existing_file_data.created_version], []))

                if readme_file_data.updated_version != "-" and existing_file_data.updated_version != "-" and readme_file_data.updated_version != existing_file_data.updated_version:
                    difference_rows.append(([readme_file_data.file_path, "Updated Version", readme_file_data.updated_version, existing_file_data.updated_version], []))

                if readme_file_data.name != "-" and existing_file_data.name != "-" and readme_file_data.name != existing_file_data.name:
                    difference_rows.append(([readme_file_data.file_path, "Name", readme_file_data.name, existing_file_data.name], []))

                if readme_file_data.username != "-" and existing_file_data.username != "-" and readme_file_data.username != existing_file_data.username:
                    difference_rows.append(([readme_file_data.file_path, "Username", readme_file_data.username, existing_file_data.username], []))

    print_table(difference_headers, difference_rows)
    print()

def main():
    current_dir = os.getcwd()
    if not current_dir.endswith("raylib/examples"):
        print("Incorrect directory. Please run the script in raylib/examples")
        print("Also, have raylib.com repository cloned in the same directory as raylib")
        return

    if not os.path.exists(current_dir.replace("raylib/examples", "raylib.com/common")):
        print(f"raylib.com directory does not exist in parent {current_dir.replace("raylib/examples", "")}.")
        print("Please have raylib.com repository cloned in the same directory as raylib")
        return

    print("# === === === PROCESSING FILES === === ===")
    print("Unchecked checkbox means it contains errors")
    print("Please review even checked checkbox")
    print("Stars are displayed using # (for full) and _ (for empty) rather than ⭐️, ★ or ☆")
    print()

    include_all_data = True

    print("## === === PROCESSING EXISTING EXAMPLES FILES === ===")
    existing_files_data = process_existing_files()
    existing_headers = ["?", "File", "Stars", "Year", "Year Start", "Year End", "Name", "Username", "Copyright Name", "Copyright Username", "Contributor Name", "Contributor Username", "Created Version", "Updated Version", "Ray Copyrighted?", "Ray Reviewed?", "Username Has '@'?"]
    existing_rows = [
        ([
            "×" if existing_file_data.data_has_errors else "√",
            existing_file_data.file_path,
            existing_file_data.stars,
            existing_file_data.year,
            existing_file_data.year_start,
            existing_file_data.year_end,
            existing_file_data.name,
            existing_file_data.username,
            existing_file_data.copyright_name,
            existing_file_data.copyright_username,
            existing_file_data.contributor_name,
            existing_file_data.contributor_username,
            existing_file_data.created_version,
            existing_file_data.updated_version,
            "YES" if existing_file_data.ray_copyrighted else "NO",
            "YES" if existing_file_data.ray_reviewed else "NO",
            "YES" if existing_file_data.username_has_no_at else "NO"
        ], []) for existing_file_data in existing_files_data if existing_file_data.data_has_errors or include_all_data
    ]
    print_table(existing_headers, existing_rows)
    print()

    print("## === === PROCESSING README.md FILE === ===")
    readme_files_data = process_readme()
    readme_headers = ["?", "File", "Stars", "Created Version", "Updated Version", "Name", "Username", "URL"]
    readme_rows = [
        ([
            "×" if readme_file_data.data_has_errors else "√",
            readme_file_data.file_path,
            readme_file_data.stars,
            readme_file_data.created_version,
            readme_file_data.updated_version,
            readme_file_data.name,
            readme_file_data.username,
            readme_file_data.user_url
        ], [[f"Line #{readme_file_data.line_number}", f"`{readme_file_data.line}`"]] if readme_file_data.data_has_errors else [])
        for readme_file_data in readme_files_data if readme_file_data.data_has_errors or include_all_data
    ]
    print_table(readme_headers, readme_rows)
    print()

    print("## === === PROCESSING examples.js FILE === ===")
    website_files_data = process_examples_js()
    website_headers = ["?", "File", "Stars"]
    website_rows = [
        ([
            "×" if website_file_data.data_has_errors else "√",
            website_file_data.file_path,
            website_file_data.stars
        ], [[f"Line #{website_file_data.line_number}", f"`{website_file_data.line}`"]] if website_file_data.data_has_errors else [])
        for website_file_data in website_files_data if website_file_data.data_has_errors or include_all_data
    ]
    print_table(website_headers, website_rows)
    print()

    print_file_comparison_results(existing_files_data, readme_files_data, website_files_data)

    print_star_comparison_results(readme_files_data, website_files_data)

    print_readme_comparison_results(existing_files_data, readme_files_data)


if __name__ == "__main__":
    main()
