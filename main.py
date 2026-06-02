import argparse
import datetime
import os
import webbrowser
from services.readings_service import ReadingsService
from services.theme_service import ThemeService
from services.ai_service import AIService
from services.email_service import EmailService

def get_liturgical_day(date_obj, season, day_name):
    """
    Approximates the liturgical day string.
    """
    def get_ordinal(n):
        ordinals = ['', 'First', 'Second', 'Third', 'Fourth', 'Fifth', 'Sixth', 'Seventh', 'Eighth', 'Ninth', 'Tenth',
                   'Eleventh', 'Twelfth', 'Thirteenth', 'Fourteenth', 'Fifteenth', 'Sixteenth', 'Seventeenth', 'Eighteenth', 
                   'Nineteenth', 'Twentieth', 'Twenty-first', 'Twenty-second', 'Twenty-third', 'Twenty-fourth', 'Twenty-fifth',
                   'Twenty-sixth', 'Twenty-seventh', 'Twenty-eighth', 'Twenty-ninth', 'Thirtieth', 'Thirty-first', 'Thirty-second', 'Thirty-third', 'Thirty-fourth']
        return ordinals[n] if n < len(ordinals) else f"{n}th"

    # Simple logic for Ordinary Time approximation
    if season == "Ordinary Time":
        # Rough calculation of week in Ordinary Time
        # In a real app, this should come from the API or a more robust library
        day_of_year = date_obj.timetuple().tm_yday
        week_num = (day_of_year // 7) + 1
        # Adjusting for 2026 specifics or using a more general offset
        # This is a placeholder for refined liturgical logic
        return f"{day_name} of the {get_ordinal(week_num)} Week in Ordinary Time"
    
    return f"{day_name} in {season}"

def main():
    parser = argparse.ArgumentParser(description="Daily Spiritual Summary Generator")
    parser.add_argument("--date", help="Date in YYYY-MM-DD format (defaults to today)")
    parser.add_argument("--dry-run", action="store_true", help="Generate HTML and open in browser without sending email")
    args = parser.parse_args()

    # Determine date
    if args.date:
        target_date = datetime.datetime.strptime(args.date, "%Y-%m-%d")
    else:
        target_date = datetime.datetime.now()
    
    date_str = target_date.strftime("%Y-%m-%d")
    day_name = target_date.strftime("%A")
    readable_today = target_date.strftime("%A, %B %d, %Y")

    print(f"🚀 Starting Daily Summary for {date_str}...")

    # 1. Fetch Readings and Saint
    readings_svc = ReadingsService()
    try:
        readings_data = readings_svc.get_readings(date_str)
        saint_data = readings_svc.get_saint(date_str)
    except Exception as e:
        print(f"❌ Error fetching readings: {e}")
        return

    # 2. Get Theme
    theme_svc = ThemeService()
    theme = theme_svc.get_theme(day_name)

    # 3. Prepare AI Context
    ai_context = {
        "today": readable_today,
        "day_of_week": day_name,
        "theme": theme,
        "season": readings_data.get("season", "Ordinary Time"),
        "first_reading": readings_data["readings"].get("firstReading", {}).get("citation"),
        "psalm": readings_data["readings"].get("psalm", {}).get("citation"),
        "second_reading": readings_data["readings"].get("secondReading", {}).get("citation"),
        "gospel": readings_data["readings"].get("gospel", {}).get("citation"),
        "has_saint": saint_data is not None,
        "saint": saint_data.get("saint") if saint_data else None
    }

    # 4. Generate AI Reflection
    print("🤖 Generating AI reflection...")
    ai_svc = AIService()
    try:
        ai_output = ai_svc.generate_reflection(ai_context)
    except Exception as e:
        print(f"❌ AI Generation failed: {e}")
        return

    # 5. Build HTML
    print("📧 Building HTML email...")
    email_svc = EmailService()
    liturgical_day = get_liturgical_day(target_date, ai_context["season"], day_name)
    
    context = {
        "liturgical_day": liturgical_day,
        "readings": readings_data["readings"],
        "theme": theme,
        "ai_output": ai_output,
        "usccbLink": readings_data["usccbLink"]
    }
    
    html_content = email_svc.render_html(context)
    subject = f"Daily Benedictine Reflection - {readable_today}"

    # 6. Action
    if args.dry_run:
        print("📁 Dry-run mode: Saving HTML locally...")
        output_path = os.path.abspath("preview.html")
        with open(output_path, "w") as f:
            f.write(html_content)
        print(f"✅ Preview saved to: {output_path}")
        webbrowser.open(f"file://{output_path}")
    else:
        print("📤 Sending email...")
        email_svc.send_email(subject, html_content)
        print("✅ Email sent successfully!")

if __name__ == "__main__":
    main()
