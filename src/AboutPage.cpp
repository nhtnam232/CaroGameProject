#include"UI.h"




void DrawAboutMenu(int screenWidth, int screenHeight, const GameAssets& assets, int selectedIndex) //selectedIndex = 0: About us, 1: Special Thanks
{
	const char* title = (selectedIndex == 0) ? "ABOUT US" : "SPECIAL THANKS";
	Rectangle panel = DrawBaseMenu(screenWidth, screenHeight, assets, title);


	//get positions
	float px = panel.x;
	float py = panel.y;
	float pw = panel.width;
	float ph = panel.height;
	float hh = 68.0f;
	float tabW = 240.0f;
	float tabH = 45.0f;
	float tab1X = px + 40.0f;
	float tab2X = px + 40.0f + tabW + 15.0f;
	float tabY = py - tabH + 5.0f;

	// Hint Bar
	const char* hint = "A/D: Navigate  ESC: Return to Menu";
	float hintY = py + ph + 14.0f;
	float hintW = 300.0f;
	float hintX = screenWidth / 2.0f - hintW / 2.0f;
	DrawHint(screenWidth, screenHeight, hintX, hintY, hintW, hint, assets);
	// Color
	Color tabActiveFill = ColorBrownRust;
	Color tabInactiveFill = ColorBrownMedium;
	Color textActive = ColorGoldSolid;
	Color textInactive = GRAY;

	// Render Tab 1 (ABOUT US)
	DrawRectangleRounded({ tab1X, tabY, tabW, tabH }, 0.2f, 4, selectedIndex == 0 ? tabActiveFill : tabInactiveFill);
	DrawRectangleRoundedLines({ tab1X, tabY, tabW, tabH }, 0.2f, 4, ColorBronzeSolid);
	DrawTextEx(assets.gameFont, "ABOUT US", { tab1X + 55, tabY + 12 }, 24, 1, selectedIndex == 0 ? textActive : textInactive);

	//Render Tab 2 (SPECIAL THANKS)
	DrawRectangleRounded({ tab2X, tabY, tabW, tabH }, 0.2f, 4, selectedIndex == 1 ? tabActiveFill : tabInactiveFill);
	DrawRectangleRoundedLines({ tab2X, tabY, tabW, tabH }, 0.2f, 4, ColorBronzeSolid);
	DrawTextEx(assets.gameFont, "SPECIAL THANKS", { tab2X + 15, tabY + 12 }, 24, 1, selectedIndex == 1 ? textActive : textInactive);

	if (selectedIndex == 0) {

		// Text
		float textX = px + 50.0f;
		float textY = py + hh + 20.0f;
		float lineSpacing = 38.0f;

		Font font = assets.gameFont;

		// Color for text
		Color cLabel = ColorGoldSolid;
		Color cValue = ColorCreamSolid;
		Color cID = ColorGoldSolid;
		Color cName = ColorCreamSolid;
		Color cUni = ColorPinkNeon;
		Color cFaculty = ColorBlueNeon;

		//School and Falcuty name
		const char* uniText = "University of Science - VNUHCM";
		const char* facultyText = "Faculty of Information Technology";
		int fontSizeSchoolText = 40;
		int fontSizeFalcutyText = 34;
		Vector2 uniSize = MeasureTextEx(font, uniText, fontSizeSchoolText, 1);
		DrawTextEx(font, uniText, { px + pw / 2.0f - uniSize.x / 2.0f, textY }, fontSizeSchoolText, 1, cUni);
		textY += 35.0f;

		Vector2 facultySize = MeasureTextEx(font, facultyText, fontSizeFalcutyText, 1);
		DrawTextEx(font, facultyText, { px + pw / 2.0f - facultySize.x / 2.0f, textY }, fontSizeFalcutyText, 1, cFaculty);
		textY += 50.0f;

		// Course
		DrawTextEx(font, "Course:", { textX, textY }, 26, 1, cLabel);
		DrawTextEx(font, "Fundamentals of Programming", { textX + 130, textY }, 26, 1, cValue);
		textY += lineSpacing;

		// Project Name
		DrawTextEx(font, "Project:", { textX, textY }, 26, 1, cLabel);
		DrawTextEx(font, "Caro Game", { textX + 130, textY }, 26, 1, cValue);
		textY += lineSpacing;

		// Instructor
		DrawTextEx(font, "Instructor:", { textX, textY }, 26, 1, cLabel);
		DrawTextEx(font, "Dr. Truong Toan Thinh", { textX + 130, textY }, 26, 1, cValue);

		//Add space
		textY += lineSpacing;
		//Team Members
		DrawTextEx(font, "Team Members:", { textX, textY }, 26, 1, cLabel);
		textY += 35.0f;
		//Counting x, y
		float cardY = textY;
		float card1X = px + 130.0f;
		float card2X = card1X + 255.0f + 30.0f;

		// Drawing card
		DrawCard(font, card1X, cardY, "24120387", "Tran Nhat Nam", "Vibe Coder", 5, ColorPinkNeon);
		DrawCard(font, card2X, cardY, "24120349", "Bui Tan Kien", "Vibe Coder", 5, ColorBlueNeon);
	}
	else { //Special Thanks

		float textY = py + hh + 40.0f;
		float textX = px + 40.0f;

		//Opening statement
		const char* openingText = "We would like to express our deepest gratitude to:";
		Vector2 openingSize = MeasureTextEx(assets.gameFont, openingText, 26, 1);
		DrawTextEx(assets.gameFont, openingText, { px + pw / 2.0f - openingSize.x / 2.0f, textY }, 26, 1, ColorCreamSolid);
		textY += 80.0f;

		DrawTextEx(assets.gameFont, "Dr. Truong Toan Thinh", { textX + 230, textY - 30 }, 30, 1, ColorGoldSolid);
		textY += 40.0f;

		float indentX = textX + 15.0f;

		DrawTextEx(assets.gameFont, "For your invaluable guidance and continuous support in the", { indentX, textY }, 24, 1, ColorCreamSolid);
		textY += 33.0f;


		DrawTextEx(assets.gameFont, "Fundamentals of Programming course. Your lessons give us the core foundation", { indentX, textY }, 24, 1, ColorCreamSolid);
		textY += 33.0f;

		DrawTextEx(assets.gameFont, "for this Caro project.", { indentX, textY }, 24, 1, ColorCreamSolid);
		textY += 33.0f;


	}
}