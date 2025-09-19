// Format the AI output into beautiful HTML email
const aiOutput = JSON.parse($input.first().json.output);
const dateInfo = $('Format Date').first().json;
const readingsData = $('Get Readings').first().json;

// Build second reading line separately to avoid quote issues
const secondReadingLine = readingsData.readings.secondReading ?
  `<strong>Second Reading:</strong> ${readingsData.readings.secondReading}<br>` :
  '';

// Handle saint section
const saintSection = aiOutput.saintOfTheDay ?
  `<div style="margin-bottom: 35px; background-color: #fff3cd; padding: 25px; border-left: 4px solid #f39c12;">
    <h3 style="color: #d68910; font-size: 1.3em; margin-bottom: 15px; text-transform: uppercase; letter-spacing: 1px; font-weight: normal;">
      Saint of the Day
    </h3>
    <p style="line-height: 1.8; font-size: 16px; color: #34495e;">
      ${aiOutput.saintOfTheDay}
    </p>
  </div>` : '';

const htmlContent = `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Daily Benedictine Reflection</title>
</head>
<body style="margin: 0; padding: 0; font-family: Georgia, 'Times New Roman', serif; background-color: #f8f9fa; color: #2c3e50;">
    <div style="max-width: 800px; margin: 0 auto; background-color: #ffffff; box-shadow: 0 2px 10px rgba(0,0,0,0.1);">
        
        <!-- Header -->
        <div style="background: linear-gradient(135deg, #34495e 0%, #2c3e50 100%); color: white; padding: 40px 20px; text-align: center;">
            <h1 style="margin: 0; font-size: 2.5em; font-weight: normal; letter-spacing: 2px;">
                Daily Benedictine Reflection
            </h1>
        </div>

        <!-- Date Navigation -->
        <div style="background-color: #ecf0f1; padding: 20px; text-align: center; border-bottom: 1px solid #bdc3c7;">
            <div style="display: inline-flex; align-items: center; gap: 30px;">
                <span style="color: #7f8c8d; font-size: 14px; text-transform: uppercase; letter-spacing: 1px;">
                    ${dateInfo.today}
                </span>
            </div>
        </div>

        <!-- Main Content -->
        <div style="padding: 40px;">
            
            <!-- Liturgical Season -->
            <div style="text-align: center; margin-bottom: 40px;">
                <h2 style="font-size: 1.8em; color: #2c3e50; margin: 0; font-weight: normal;">
                    ${readingsData.season} • ${dateInfo.theme}
                </h2>
                <div style="width: 60px; height: 3px; background-color: #3498db; margin: 20px auto;"></div>
            </div>

            <!-- Spiritual Focus -->
            <div style="margin-bottom: 35px;">
                <h3 style="color: #2980b9; font-size: 1.3em; margin-bottom: 15px; text-transform: uppercase; letter-spacing: 1px; font-weight: normal;">
                    Today's Spiritual Focus
                </h3>
                <p style="line-height: 1.8; font-size: 16px; color: #34495e;">
                    ${aiOutput.spiritualFocus}
                </p>
            </div>

            <!-- Liturgical Readings -->
            <div style="margin-bottom: 35px; background-color: #f8f9fa; padding: 25px; border-left: 4px solid #3498db;">
                <h3 style="color: #2980b9; font-size: 1.3em; margin-bottom: 15px; text-transform: uppercase; letter-spacing: 1px; font-weight: normal;">
                    Today's Readings
                </h3>
                <div style="margin-bottom: 15px;">
                    <strong>First Reading:</strong> ${readingsData.readings.firstReading}<br>
                    <strong>Psalm:</strong> ${readingsData.readings.psalm}<br>
                    ${secondReadingLine}
                    <strong>Gospel:</strong> ${readingsData.readings.gospel}
                </div>
                <p style="line-height: 1.8; font-size: 16px; color: #34495e; margin-top: 20px;">
                    ${aiOutput.liturgicalReadings}
                </p>
            </div>

            <!-- Saint Section -->
            ${saintSection}

            <!-- Reflection -->
            <div style="margin-bottom: 35px;">
                <h3 style="color: #2980b9; font-size: 1.3em; margin-bottom: 15px; text-transform: uppercase; letter-spacing: 1px; font-weight: normal;">
                    Reflection & Challenge
                </h3>
                <p style="line-height: 1.8; font-size: 16px; color: #34495e;">
                    ${aiOutput.reflection}
                </p>
                <p style="line-height: 1.8; font-size: 16px; color: #34495e; margin-top: 20px;">
                    <strong>Practical Spiritual Challenge:</strong>
                    ${aiOutput.practicalChallenge}
                </p>
            </div>

            <!-- Prayer -->
            <div style="background: linear-gradient(135deg, #f8f9fa 0%, #ecf0f1 100%); padding: 30px; border-radius: 8px; text-align: center; border: 1px solid #bdc3c7;">
                <h3 style="color: #2980b9; font-size: 1.3em; margin-bottom: 15px; text-transform: uppercase; letter-spacing: 1px; font-weight: normal;">
                    Daily Prayer
                </h3>
                <p style="line-height: 1.8; font-size: 16px; color: #34495e; font-style: italic;">
                    ${aiOutput.prayer}
                </p>
            </div>

        </div>

        <!-- Footer -->
        <div style="background-color: #34495e; color: #ecf0f1; padding: 30px; text-align: center;">
            <p style="margin: 0; font-size: 14px;">
                Ora et Labora<br>
                <a href="${readingsData.usccbLink}" style="color: #3498db; text-decoration: none;">View Today's Official Readings</a>
            </p>
        </div>

    </div>
</body>
</html>`;

return {
  htmlContent: htmlContent,
  subject: `Daily Benedictine Reflection - ${dateInfo.today}`,
  aiOutput: aiOutput,
  dateInfo: dateInfo
};