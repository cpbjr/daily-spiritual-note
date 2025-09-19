// Los Angeles Dodgers 2025 Season Schedule - Generated from MLB API
// Returns game information for any given date
// Complete schedule from August 25, 2025 through end of season
// All times shown in Mountain Time Zone

const dateInfo = $json; // Expecting dateInfo.monthDay format like "8/24"

const dodgersSchedule = {
  // Complete 2025 Schedule from Today Forward (Mountain Time Zone)
  '8/25': { opponent: 'Cincinnati Reds', venue: 'home', time: '8:10 PM', type: 'Regular Season' },
  '8/26': { opponent: 'Cincinnati Reds', venue: 'home', time: '8:10 PM', type: 'Regular Season' },
  '8/27': { opponent: 'Cincinnati Reds', venue: 'home', time: '6:40 PM', type: 'Regular Season' },
  '8/29': { opponent: 'Arizona Diamondbacks', venue: 'home', time: '8:10 PM', type: 'Regular Season' },
  '8/30': { opponent: 'Arizona Diamondbacks', venue: 'home', time: '7:10 PM', type: 'Regular Season' },
  '8/31': { opponent: 'Arizona Diamondbacks', venue: 'home', time: '2:10 PM', type: 'Regular Season' },
  '9/2': { opponent: 'Pittsburgh Pirates', venue: 'away', time: '4:40 PM', type: 'Regular Season' },
  '9/3': { opponent: 'Pittsburgh Pirates', venue: 'away', time: '4:40 PM', type: 'Regular Season' },
  '9/4': { opponent: 'Pittsburgh Pirates', venue: 'away', time: '4:40 PM', type: 'Regular Season' },
  '9/5': { opponent: 'Baltimore Orioles', venue: 'away', time: '5:05 PM', type: 'Regular Season' },
  '9/6': { opponent: 'Baltimore Orioles', venue: 'away', time: '5:05 PM', type: 'Regular Season' },
  '9/7': { opponent: 'Baltimore Orioles', venue: 'away', time: '11:35 AM', type: 'Regular Season' },
  '9/8': { opponent: 'Colorado Rockies', venue: 'home', time: '8:10 PM', type: 'Regular Season' },
  '9/9': { opponent: 'Colorado Rockies', venue: 'home', time: '8:10 PM', type: 'Regular Season' },
  '9/10': { opponent: 'Colorado Rockies', venue: 'home', time: '8:10 PM', type: 'Regular Season' },
  '9/12': { opponent: 'San Francisco Giants', venue: 'away', time: '8:15 PM', type: 'Regular Season' },
  '9/13': { opponent: 'San Francisco Giants', venue: 'away', time: '7:05 PM', type: 'Regular Season' },
  '9/14': { opponent: 'San Francisco Giants', venue: 'away', time: '2:05 PM', type: 'Regular Season' },
  '9/15': { opponent: 'Philadelphia Phillies', venue: 'home', time: '8:10 PM', type: 'Regular Season' },
  '9/16': { opponent: 'Philadelphia Phillies', venue: 'home', time: '8:10 PM', type: 'Regular Season' },
  '9/17': { opponent: 'Philadelphia Phillies', venue: 'home', time: '8:10 PM', type: 'Regular Season' },
  '9/18': { opponent: 'San Francisco Giants', venue: 'home', time: '8:10 PM', type: 'Regular Season' },
  '9/19': { opponent: 'San Francisco Giants', venue: 'home', time: '8:10 PM', type: 'Regular Season' },
  '9/20': { opponent: 'San Francisco Giants', venue: 'home', time: '7:10 PM', type: 'Regular Season' },
  '9/21': { opponent: 'San Francisco Giants', venue: 'home', time: '2:10 PM', type: 'Regular Season' },
  '9/23': { opponent: 'Arizona Diamondbacks', venue: 'away', time: '7:40 PM', type: 'Regular Season' },
  '9/24': { opponent: 'Arizona Diamondbacks', venue: 'away', time: '7:40 PM', type: 'Regular Season' },
  '9/25': { opponent: 'Arizona Diamondbacks', venue: 'away', time: '1:40 PM', type: 'Regular Season' },
  '9/26': { opponent: 'Seattle Mariners', venue: 'away', time: '7:40 PM', type: 'Regular Season' },
  '9/27': { opponent: 'Seattle Mariners', venue: 'away', time: '7:40 PM', type: 'Regular Season' },
  '9/28': { opponent: 'Seattle Mariners', venue: 'away', time: '1:10 PM', type: 'Regular Season' }
};

// Get game info for the current date
const currentDate = dateInfo.monthDay || dateInfo;
const todayGame = dodgersSchedule[currentDate];

// Get yesterday's game (for the daily summary)
const yesterday = dateInfo.yesterdayMonthDay || (() => {
  const today = new Date();
  const yesterday = new Date(today.getTime() - 24*60*60*1000);
  return `${yesterday.getMonth() + 1}/${yesterday.getDate()}`;
})();

const yesterdayGame = dodgersSchedule[yesterday];

// Helper function to determine if we should look for game results
function shouldCheckGameResults(gameInfo) {
  if (!gameInfo) return false;
  if (gameInfo.type === 'Off Day') return false;
  return true;
}

// Check if we're in postseason period (October 1st or later)
function isPostseason(dateString) {
  const [month, day] = dateString.split('/').map(Number);
  return month >= 10; // October, November, December
}

// Return comprehensive game information with hybrid logic
let result;

if (isPostseason(currentDate) || isPostseason(yesterday)) {
  // POSTSEASON MODE: Use dynamic MLB API for all playoff games
  result = {
    mode: 'dynamic',
    season: 'postseason',
    today: {
      date: currentDate,
      useDynamicAPI: true,
      isPostseason: isPostseason(currentDate)
    },
    yesterday: {
      date: yesterday,
      useDynamicAPI: true,
      isPostseason: isPostseason(yesterday),
      shouldCheckResults: true // Always check API for playoff results
    },
    // Always use MLB API during postseason
    mlbApiNeeded: true,
    message: 'Using dynamic MLB API for postseason games'
  };
} else {
  // REGULAR SEASON MODE: Use static schedule
  result = {
    mode: 'static',
    season: 'regular',
    today: {
      date: currentDate,
      game: todayGame || null,
      hasGame: !!todayGame,
      isPostseason: false
    },
    yesterday: {
      date: yesterday,
      game: yesterdayGame || null,
      hasGame: !!yesterdayGame,
      shouldCheckResults: shouldCheckGameResults(yesterdayGame),
      isPostseason: false
    },
    // MLB API query helpers for regular season
    mlbApiNeeded: shouldCheckGameResults(yesterdayGame),
    apiQuery: yesterdayGame ? {
      opponent: yesterdayGame.opponent,
      venue: yesterdayGame.venue,
      expectedTime: yesterdayGame.time,
      homeAway: yesterdayGame.venue === 'home' ? 'vs' : '@'
    } : null
  };
}

console.log(`=== SCHEDULE CHECK FOR ${currentDate} ===`);
console.log(`Mode: ${result.mode} (${result.season} season)`);
console.log(`MLB API needed: ${result.mlbApiNeeded}`);

if (result.mode === 'static') {
  // Regular season static lookups
  console.log(`- Today has game: ${result.today.hasGame}`);
  console.log(`- Yesterday (${yesterday}) had game: ${result.yesterday.hasGame}`);
  
  if (result.yesterday.game) {
    console.log(`- Yesterday's game: ${result.apiQuery.homeAway} ${result.yesterday.game.opponent} at ${result.yesterday.game.time}`);
  }
  
  if (result.today.game) {
    console.log(`- Today's game: ${result.today.game.venue === 'home' ? 'vs' : '@'} ${result.today.game.opponent} at ${result.today.game.time}`);
  }
} else {
  // Postseason dynamic mode
  console.log(`- Postseason mode: ${result.message}`);
  console.log(`- Today is postseason: ${result.today.isPostseason}`);
  console.log(`- Yesterday is postseason: ${result.yesterday.isPostseason}`);
  console.log(`- Will use MLB API for live playoff schedule/results`);
}

return result;