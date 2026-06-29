import requests
import re
import urllib.parse
from typing import Dict, Optional
from config import settings

class ReadingsService:
    @staticmethod
    def get_readings(date_str: str) -> Dict:
        """
        date_str in format YYYY-MM-DD
        """
        year, month, day = date_str.split('-')
        api_url = f"{settings.READINGS_API_BASE_URL}/readings/{year}/{month}-{day}.json"

        response = requests.get(api_url, timeout=30)
        response.raise_for_status()
        data = response.json()

        # Add deep links to readings
        if 'readings' in data:
            for key, citation in data['readings'].items():
                data['readings'][key] = {
                    "citation": citation,
                    "youversion_link": ReadingsService.generate_youversion_link(citation)
                }

        return data

    @staticmethod
    def get_saint(date_str: str) -> Optional[Dict]:
        year, month, day = date_str.split('-')
        api_url = f"{settings.READINGS_API_BASE_URL}/liturgical-calendar/{year}/{month}-{day}.json"

        response = requests.get(api_url, timeout=30)
        if response.status_code == 404:
            return None
        response.raise_for_status()
        data = response.json()

        # Normalize the API's 'celebration' field to the 'saint' key used downstream
        if 'celebration' in data:
            return {"saint": data['celebration']}
        return None

    @staticmethod
    def generate_youversion_link(citation: str) -> str:
        """
        Converts a citation like 'Luke 18:35-43' or '1 Maccabees 1:10-15'
        to 'https://bible.com/bible/2015/luk.18.35-43.NRSVCI'
        (version id and abbreviation come from settings).
        """
        # Mapping common Catholic books to YouVersion abbreviations
        book_map = {
            "Matthew": "MAT", "Mark": "MRK", "Luke": "LUK", "John": "JHN",
            "Acts": "ACT", "Romans": "ROM", "1 Corinthians": "1CO", "2 Corinthians": "2CO",
            "Galatians": "GAL", "Ephesians": "EPH", "Philippians": "PHP", "Colossians": "COL",
            "1 Thessalonians": "1TH", "2 Thessalonians": "2TH", "1 Timothy": "1TI", "2 Timothy": "2TI",
            "Titus": "TIT", "Philemon": "PHM", "Hebrews": "HEB", "James": "JAS",
            "1 Peter": "1PE", "2 Peter": "2PE", "1 John": "1JN", "2 John": "2JN",
            "3 John": "3JN", "Jude": "JUD", "Revelation": "REV",
            "Genesis": "GEN", "Exodus": "EXO", "Leviticus": "LEV", "Numbers": "NUM",
            "Deuteronomy": "DEU", "Joshua": "JOS", "Judges": "JDG", "Ruth": "RUT",
            "1 Samuel": "1SA", "2 Samuel": "2SA", "1 Kings": "1KI", "2 Kings": "2KI",
            "1 Chronicles": "1CH", "2 Chronicles": "2CH", "Ezra": "EZR", "Nehemiah": "NEH",
            "Esther": "EST", "Job": "JOB", "Psalms": "PSA", "Psalm": "PSA", "Proverbs": "PRO",
            "Ecclesiastes": "ECC", "Song of Solomon": "SNG", "Isaiah": "ISA", "Jeremiah": "JER",
            "Lamentations": "LAM", "Ezekiel": "EZK", "Daniel": "DAN", "Hosea": "HOS",
            "Joel": "JOE", "Amos": "AMO", "Obadiah": "OBA", "Jonah": "JON", "Micah": "MIC",
            "Nahum": "NAH", "Habakkuk": "HAB", "Zephaniah": "ZEP", "Haggai": "HAG",
            "Zechariah": "ZEC", "Malachi": "MAL",
            # Deuterocanonical
            "Tobit": "TOB", "Judith": "JDT", "Wisdom": "WIS", "Sirach": "SIR",
            "Baruch": "BAR", "1 Maccabees": "1MA", "2 Maccabees": "2MA"
        }

        # Regex to split book and reference
        # Handles books starting with a number (1 John) or multi-word books
        match = re.match(r"^((?:\d\s)?[a-zA-Z\s]+?)\s(\d+.*)$", citation.strip())
        if not match:
            return f"https://bible.com/search/bible?q={citation}" # Fallback

        book_name = match.group(1).strip()
        reference = match.group(2).strip()

        # Get abbreviation, fallback to first 3 letters if not in map
        abbr = book_map.get(book_name, book_name[:3].upper()).lower()

        # Format: bible.com/bible/{VERSION}/{ABBR}.{CHAPTER}.{VERSES}.{BIBLE_VERSION_ABBR}
        # We need to replace colons with dots and spaces with nothing
        ref_formatted = reference.replace(":", ".").replace(" ", "")

        return f"https://bible.com/bible/{settings.BIBLE_VERSION_ID}/{abbr}.{ref_formatted}.{settings.BIBLE_VERSION_ABBR}"


def build_celebration_link(name: str) -> str:
    """
    Build a reliable URL for a Catholic celebration or saint.

    Grokipedia (https://grokipedia.com) was evaluated but returns HTTP 404 for
    all /page/* URLs — including well-known saints like Saint Thomas More and
    Saint Peter Claver — so it is not usable as a source.  We fall back
    directly to a Wikipedia search URL, which always resolves: exact article
    matches redirect to the article, non-matches show search results.
    """
    return f"https://en.wikipedia.org/w/index.php?search={urllib.parse.quote(name)}"
