class ThemeService:
    THEMES = {
        'Sunday': 'Worship & Community',
        'Monday': 'Humility & Simplicity',
        'Tuesday': 'Stability',
        'Wednesday': 'Listening',
        'Thursday': 'Hospitality & Kindness',
        'Friday': 'Sacrifice & Repentance',
        'Saturday': 'Rest & Beauty'
    }

    @staticmethod
    def get_theme(day_of_week: str) -> str:
        return ThemeService.THEMES.get(day_of_week, 'Prayer')
