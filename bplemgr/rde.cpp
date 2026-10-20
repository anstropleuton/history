////////////////////////////////////////////////////////////////////////////////
//                        Copyright (c) Anstro Plueton                        //
////////////////////////////////////////////////////////////////////////////////
//   Permission is hereby granted, free of charge, to any person obtaining a  //
// copy of this software and associated documentation files (the "Software"), //
// to deal in the Software without restriction, including without limitation  //
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,  //
//    and/or sell copies of the Software, and to permit persons to whom the   //
//    Software is furnished to do so, subject to the following conditions:    //
//                                                                            //
//   The above copyright notice and this permission notice shall be included  //
//           in all copies or substantial portions of the Software.           //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,  //
//   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL  //
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //
//   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  //
//     FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER    //
//                          DEALINGS IN THE SOFTWARE.                         //
////////////////////////////////////////////////////////////////////////////////
//                           版权 (C) Anstro Pleuton                           //
////////////////////////////////////////////////////////////////////////////////
//       特此向获得的任何人免费给予许可。该软件和相关文件文件(“软件”)的副本，       //
//       不受限制地处理软件，包括不受限制使用、复制、修改、合并、发布、分发、       //
//   转接许可的权利，和或出售软件的副本，并允许其软件提供这样做，但须符合下列条件：  //
//                                                                            //
//             上述版权公告和本许可通知应包括在软件的所有副本或大部分。            //
//                                                                            //
//             该软件是按原样提供的，没有任何形式的保证，快递或隐含的，            //
//               包括但不限于适销性的保证，适合某一特定目的和不侵犯。              //
//       在任何情况下都不能作者或版权持有人对任何索赔、损害赔偿或其他法律责任，      //
//                     不论是在合同诉讼、侵权行为或其他情况下，                   //
//                    产生与软件或使用或其他有关的或者软件交易。                  //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "rde.hpp"

void rde::Instance::generateBlurShaderSourceCode()
{
	blurShaderSourceCode =
		"#ifdef GL_ES\n"
		"precision mediump float;\n"
		"#endif\n"
		"\n"
		"uniform sampler2D texture0;\n"
		"uniform vec2 u_resolution;\n"
		"\n"
		"uniform float u_blurRadius;\n"
		"const float c_blurQuality = ";
	blurShaderSourceCode += TextFormat("%f", blurQuality);
	blurShaderSourceCode +=
		";\n"
		"\n"
		"void main()\n"
		"{\n"
		"	float blurRadius = u_blurRadius / c_blurQuality * 1.57079632679;\n"
		"	vec4 blurColor = vec4(0.);\n"
		"\n"
		"	for (float alpha = -c_blurQuality / 2.; alpha <= c_blurQuality / 2.; alpha++)\n"
		"		for (float beta = -c_blurQuality / 2.; beta <= c_blurQuality / 2.; beta++)\n"
		"			blurColor += texture2D(texture0, (gl_FragCoord.xy + vec2(alpha * blurRadius, beta * blurRadius)) / u_resolution);\n"
		"	\n"
		"	blurColor /= c_blurQuality * c_blurQuality + 2. * c_blurQuality + 1.;\n"
		"\n"
		"	gl_FragColor = blurColor;\n"
		"}\n";
}

void rde::Instance::generateShadowShaderSourceCode()
{
	shadowShaderSourceCode =
		"#ifdef GL_ES\n"
		"precision mediump float;\n"
		"#endif\n"
		"\n"
		"uniform sampler2D texture0;\n"
		"uniform vec2 u_resolution;\n"
		"\n"
		"uniform vec4 u_rectangle;\n"
		"uniform float u_roundness;\n"
		"uniform float u_shadowRadius;\n"
		"uniform float u_shadowSize;\n"
		"uniform vec4 u_shadowColor;\n"
		"uniform vec2 u_shadowOffset;\n"
		"\n"
		"// Not by me, Idk how it works but it does work xD\n"
		"// https://iquilezles.org/articles/distfunctions\n"
		"float roundedBoxSDF(vec2 centerPosition,vec2 size,float radius)\n"
		"{\n"
		"	if(min(size.x, size.y) < radius)\n"
		"		radius = min(size.x, size.y);\n"
		"	return length(max(abs(centerPosition)-size+radius,0.))-radius;\n"
		"}\n"
		"void main()\n"
		"{\n"
		"	// Rounded rectangle calculation\n"
		"	vec2 size = u_rectangle.zw / 2.;\n"
		"	vec2 position = gl_FragCoord.xy - u_rectangle.xy - size;\n"
		"	float rectangleRoundedBoxSDF = smoothstep(1., 0., roundedBoxSDF(position, size, u_roundness));\n"
		"	float shadowRoundedBoxSDF = 1. + rectangleRoundedBoxSDF - smoothstep(0., u_shadowRadius, roundedBoxSDF(position - u_shadowOffset, size + u_shadowSize, u_roundness));\n"
		"\n"
		"	vec4 backgroundColor = vec4(texture2D(texture0, gl_FragCoord.xy/u_resolution).rgb * rectangleRoundedBoxSDF, rectangleRoundedBoxSDF);\n"
		"\n"
		"	gl_FragColor = backgroundColor + vec4(u_shadowColor * shadowRoundedBoxSDF);\n"
		"}\n";
}

void rde::Instance::generateGradientShaderSourceCode()
{
	// Yep it's a short shader
	gradientShaderSourceCode =
		"#ifdef GL_ES\n"
		"precision mediump float;\n"
		"#endif\n"
		"\n"
		"uniform vec2 u_position;\n"
		"uniform vec4 u_colorOne;\n"
		"uniform vec4 u_colorTwo;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor=mix(u_colorOne,u_colorTwo,((gl_FragCoord.x-u_position.x)/u_position.y)*2.);\n"
		"}\n";
}

void rde::Instance::generatewidgitShaderSourceCode()
{
	widgitShaderSourceCode =
		"#ifdef GL_ES\n"
		"precision mediump float;\n"
		"#endif\n"
		"\n"
		"uniform float u_roundness;\n"
		"uniform vec4 u_rectangle;\n"
		"uniform vec4 u_color;\n"
		"\n"
		"// Not by me, Idk how it works but it does work xD\n"
		"// https://iquilezles.org/articles/distfunctions\n"
		"float roundedBoxSDF(vec2 centerPosition, vec2 size, float radius)\n"
		"{\n"
		"	if (min(size.x, size.y) < radius)\n"
		"		radius = min(size.x, size.y);\n"
		"	return length(max(abs(centerPosition) - size + radius, 0.)) - radius;\n"
		"}\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = u_color * smoothstep(1., 0., roundedBoxSDF(gl_FragCoord.xy - u_rectangle.xy - u_rectangle.zw / 2., u_rectangle.zw / 2., u_roundness));\n"
		"}\n";
}

void rde::Instance::load(bool a_useShader)
{
	useShader = a_useShader;

	if (useShader)
	{
		generateShadowShaderSourceCode();
		generateBlurShaderSourceCode();
		generateGradientShaderSourceCode();
		generatewidgitShaderSourceCode();

		blurShader = LoadShaderFromMemory(nullptr, blurShaderSourceCode.c_str());
		shadowShader = LoadShaderFromMemory(nullptr, shadowShaderSourceCode.c_str());
		gradientShader = LoadShaderFromMemory(nullptr, gradientShaderSourceCode.c_str());
		widgitShader = LoadShaderFromMemory(nullptr, widgitShaderSourceCode.c_str());

		blurShaderSourceCode.clear();
		shadowShaderSourceCode.clear();
		gradientShaderSourceCode.clear();
		widgitShaderSourceCode.clear();

		setGuiScale(20.0f);

		setBlurRadius(20.0f);
		setBlurQuality(10.0f);

		setRoundness(0.5f);
		setThickness(0.5f);
		setBorderSize(0.25f);

		setShadowRadius(50.0f);
		setShadowSize(-30.0f);
		setShadowColor(ColorAlpha(BLACK, 0.5f));
		setShadowOffset({0.0f, -20.0f});

		setShaderResolution({float(GetScreenWidth()), float(GetScreenHeight())});

		backgroundOne = LoadRenderTexture(resolution[0], resolution[1]);
		backgroundTwo = LoadRenderTexture(resolution[0], resolution[1]);

		windowMinWidth = 2.0f;
		windowMinHeight = 2.0f;
	}
}

void rde::Instance::unload()
{
	if (blurShader.id != 0)
		UnloadShader(blurShader);
	if (shadowShader.id != 0)
		UnloadShader(shadowShader);
	if (gradientShader.id != 0)
		UnloadShader(gradientShader);
	if (widgitShader.id != 0)
		UnloadShader(widgitShader);

	if (backgroundOne.id != 0)
		UnloadRenderTexture(backgroundOne);
	if (backgroundTwo.id != 0)
		UnloadRenderTexture(backgroundTwo);

	for (int index = 0; index < windows.size(); index++)
		delete windows[index];
	windows.clear();
}

void rde::Instance::update(bool a_updateResolition)
{
	if (utils::isKeyLongPressed(KEY_Q))
		setGuiScale(getGuiScale() + 0.125f);
	if (utils::isKeyLongPressed(KEY_A))
		setGuiScale(getGuiScale() - 0.125f);
	if (utils::isKeyLongPressed(KEY_W))
		setRoundness(getRoundness() + 0.125f);
	if (utils::isKeyLongPressed(KEY_S))
		setRoundness(getRoundness() - 0.125f);
	if (utils::isKeyLongPressed(KEY_E))
		setBorderSize(getBorderSize() + 0.125f);
	if (utils::isKeyLongPressed(KEY_D))
		setBorderSize(getBorderSize() - 0.125f);

	if (IsWindowResized() && a_updateResolition)
	{
		setShaderResolution({float(GetScreenWidth()), float(GetScreenHeight())});

		UnloadRenderTexture(backgroundOne);
		UnloadRenderTexture(backgroundTwo);

		backgroundOne = LoadRenderTexture(resolution[0], resolution[1]);
		backgroundTwo = LoadRenderTexture(resolution[0], resolution[1]);
	}

	for (int index = 0; index < windows.size(); index++)
		windows[index]->update();
}

void rde::Instance::draw(bool a_showDebugBounds, bool a_forceRedrawAll)
{
	for (int index = 0; index < windows.size(); index++)
		windows[index]->draw(a_showDebugBounds, a_forceRedrawAll);
	ClearBackground(BLANK);
	DrawTexturePro(backgroundOne.texture, {0, 0, float(backgroundOne.texture.width), float(-backgroundOne.texture.height)}, {0, 0, resolution[0], resolution[1]}, {0}, 0, WHITE);
	redrawing = false;
}

void rde::Instance::beginBackground()
{
	if (!useShader)
		return;
	BeginTextureMode(backgroundOne);
}

void rde::Instance::endBackground()
{
	if (!useShader)
		return;
	EndTextureMode();
	redrawing = true;
}

Vector2 rde::Instance::getShaderResolution()
{
	return {resolution[0], resolution[1]};
}

void rde::Instance::setShaderResolution(Vector2 a_resolution)
{
	resolution[0] = a_resolution.x;
	resolution[1] = a_resolution.y;
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "u_resolution"), resolution, SHADER_UNIFORM_VEC2);
	SetShaderValue(blurShader, GetShaderLocation(blurShader, "u_resolution"), resolution, SHADER_UNIFORM_VEC2);
}

float rde::Instance::getGuiScale()
{
	return guiScale;
}

void rde::Instance::setGuiScale(float a_guiScale)
{
	guiScale = a_guiScale;
	if (guiScale < 10.0f)
		guiScale = 10.0f;
	setRoundness(getRoundness());
	setThickness(getThickness());
}

float rde::Instance::getBlurRadius()
{
	return blurRadius;
}

void rde::Instance::setBlurRadius(float a_blurRadius)
{
	blurRadius = a_blurRadius;
	SetShaderValue(blurShader, GetShaderLocation(blurShader, "u_blurRadius"), &blurRadius, SHADER_UNIFORM_FLOAT);
}

float rde::Instance::getBlurQuality()
{
	return blurQuality;
}

void rde::Instance::setBlurQuality(float a_BlurQuality)
{
	blurQuality = a_BlurQuality;

	generateBlurShaderSourceCode();
	UnloadShader(blurShader);
	blurShader = LoadShaderFromMemory(nullptr, blurShaderSourceCode.c_str());
	blurShaderSourceCode.clear();

	setBlurRadius(getBlurRadius());
	// setBlurQuality(getBlurQuality());

	setShaderResolution({float(GetScreenWidth()), float(GetScreenHeight())});
}

float rde::Instance::getRoundness()
{
	return roundness;
}

void rde::Instance::setRoundness(float a_roundness)
{
	roundness = a_roundness;
	if (roundness > 1.0f)
		roundness = 1.0f;
	if (roundness < 0.0f)
		roundness = 0.0f;
	float localRoundness = roundness * guiScale;
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "u_roundness"), &localRoundness, SHADER_UNIFORM_FLOAT);
}

float rde::Instance::getThickness()
{
	return thickness;
}

void rde::Instance::setThickness(float a_thickness)
{
	thickness = a_thickness;
}

float rde::Instance::getBorderSize()
{
	return borderSize;
}

void rde::Instance::setBorderSize(float a_borderSize)
{
	borderSize = a_borderSize;
}

float rde::Instance::getShadowRadius()
{
	return shadowRadius;
}

void rde::Instance::setShadowRadius(float a_shadowRadius)
{
	shadowRadius = a_shadowRadius;
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "u_shadowRadius"), &shadowRadius, SHADER_UNIFORM_FLOAT);
}

float rde::Instance::getShadowSize()
{
	return shadowSize;
}

void rde::Instance::setShadowSize(float a_shadowSize)
{
	shadowSize = a_shadowSize;
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "u_shadowSize"), &shadowSize, SHADER_UNIFORM_FLOAT);
}

Color rde::Instance::getShadowColor()
{
	return Color{(unsigned char)(shadowColor[0] * 255.0f), (unsigned char)(shadowColor[1] * 255.0f), (unsigned char)(shadowColor[2] * 255.0f), (unsigned char)(shadowColor[3] * 255.0f)};
}

void rde::Instance::setShadowColor(Color a_shadowColor)
{
	shadowColor[0] = a_shadowColor.r / 255.0f;
	shadowColor[1] = a_shadowColor.g / 255.0f;
	shadowColor[2] = a_shadowColor.b / 255.0f;
	shadowColor[3] = a_shadowColor.a / 255.0f;
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "u_shadowColor"), shadowColor, SHADER_UNIFORM_VEC4);
}

Vector2 rde::Instance::getShadowOffset()
{
	return Vector2{shadowOffset[0], shadowOffset[1]};
}

void rde::Instance::setShadowOffset(Vector2 a_shadowOffset)
{
	shadowOffset[0] = a_shadowOffset.x;
	shadowOffset[1] = a_shadowOffset.y;
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "u_shadowOffset"), shadowOffset, SHADER_UNIFORM_VEC2);
}

rde::Instance::Window::Window(Instance &a_instance, Rectangle a_bounds, std::vector<bool> a_config)
	: bounds(a_bounds), instance(a_instance), config(a_config)
{
	for (int index = 0; index < int(WindowConfiguration::MAX); index++)
		widgits.push_back(new WidgitTemplate(instance, this, {-1.0f, 0.0f, 0.0f, 0.0f}, RED, BLUE, {0}, 0.5f));

	// Main border
	widgits[0]->mulColor = {0.75f, 0.75f, 0.75f, 0.75f};

	widgits[0]->mulColor = {0.5f, 0.5f, 0.5f, 0.5f};
	widgits[0]->mulColor = {0.12f, 0.12f, 0.12f, 0.12f};
	widgits[0]->mulColor = {0.12f, 0.12f, 0.12f, 0.12f};
	widgits[0]->mulColor = {0.12f, 0.12f, 0.12f, 0.12f};
	widgits[0]->mulColor = {0.12f, 0.12f, 0.12f, 0.12f};
	widgits[0]->mulColor = {0.12f, 0.12f, 0.12f, 0.12f};
}

rde::Instance::Window::~Window()
{
	for (int index = 0; index < widgits.size(); index++)
		delete widgits[index];
	widgits.clear();
}

void rde::Instance::Window::update()
{
	updateConfigBounds();
	// Yep, so many if statements again
	visualState = WindowState::OutsideWindow;
	if (CheckCollisionPointRec(GetMousePosition(), configBounds[0]))
		visualState = WindowState::InsideWindow;
	if (CheckCollisionPointRec(GetMousePosition(), configBounds[1]))
		visualState = WindowState::ResizingTop;
	if (CheckCollisionPointRec(GetMousePosition(), configBounds[2]))
		visualState = WindowState::ResizingBottom;
	if (CheckCollisionPointRec(GetMousePosition(), configBounds[3]))
		visualState = WindowState::ResizingLeft;
	if (CheckCollisionPointRec(GetMousePosition(), configBounds[4]))
		visualState = WindowState::ResizingRight;
	if (CheckCollisionPointRec(GetMousePosition(), configBounds[5]))
		visualState = WindowState::ResizingTopLeft;
	if (CheckCollisionPointRec(GetMousePosition(), configBounds[6]))
		visualState = WindowState::ResizingBottomLeft;
	if (CheckCollisionPointRec(GetMousePosition(), configBounds[7]))
		visualState = WindowState::ResizingTopRight;
	if (CheckCollisionPointRec(GetMousePosition(), configBounds[8]))
		visualState = WindowState::ResizingBottomRight;

	if (instance.roundness > 0.5f)
	{
		if (CheckCollisionPointRec(GetMousePosition(), configBounds[10]))
			visualState = WindowState::ResizingTopLeft;
		if (CheckCollisionPointRec(GetMousePosition(), configBounds[11]))
			visualState = WindowState::ResizingTopLeft;
		if (CheckCollisionPointRec(GetMousePosition(), configBounds[12]))
			visualState = WindowState::ResizingBottomLeft;
		if (CheckCollisionPointRec(GetMousePosition(), configBounds[13]))
			visualState = WindowState::ResizingBottomLeft;
		if (CheckCollisionPointRec(GetMousePosition(), configBounds[14]))
			visualState = WindowState::ResizingTopRight;
		if (CheckCollisionPointRec(GetMousePosition(), configBounds[15]))
			visualState = WindowState::ResizingTopRight;
		if (CheckCollisionPointRec(GetMousePosition(), configBounds[16]))
			visualState = WindowState::ResizingBottomRight;
		if (CheckCollisionPointRec(GetMousePosition(), configBounds[17]))
			visualState = WindowState::ResizingBottomRight;
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		initialMousePosition = GetMousePosition();
		initialBounds = bounds;
		windowState = visualState;
	}

	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		windowState = WindowState::OutsideWindow;

	// Resize the bounds accordingly
	if (GetMouseDelta().x != 0 || GetMouseDelta().y != 0)
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			requireRedrawing = true;

		switch (windowState)
		{
		case WindowState::ResizingTop:
			bounds.y = initialBounds.y + GetMousePosition().y - initialMousePosition.y;
			bounds.height = initialBounds.height + initialMousePosition.y - GetMousePosition().y;
			if (bounds.height < instance.windowMinHeight * instance.guiScale)
			{
				bounds.y = initialBounds.y + initialBounds.height - instance.windowMinHeight * instance.guiScale;
				bounds.height = instance.windowMinHeight * instance.guiScale;
			}
			break;

		case WindowState::ResizingBottom:
			bounds.height = initialBounds.height - initialMousePosition.y + GetMousePosition().y;
			if (bounds.height < instance.windowMinHeight * instance.guiScale)
				bounds.height = instance.windowMinHeight * instance.guiScale;
			break;

		case WindowState::ResizingLeft:
			bounds.x = initialBounds.x + GetMousePosition().x - initialMousePosition.x;
			bounds.width = initialBounds.width + initialMousePosition.x - GetMousePosition().x;
			if (bounds.width < instance.windowMinWidth * instance.guiScale)
			{
				bounds.x = initialBounds.x + initialBounds.width - instance.windowMinWidth * instance.guiScale;
				bounds.width = instance.windowMinWidth * instance.guiScale;
			}
			break;

		case WindowState::ResizingRight:
			bounds.width = initialBounds.width - initialMousePosition.x + GetMousePosition().x;
			if (bounds.width < instance.windowMinWidth * instance.guiScale)
				bounds.width = instance.windowMinWidth * instance.guiScale;
			break;

		case WindowState::ResizingTopLeft:
			bounds.y = initialBounds.y + GetMousePosition().y - initialMousePosition.y;
			bounds.height = initialBounds.height + initialMousePosition.y - GetMousePosition().y;
			if (bounds.height < instance.windowMinHeight * instance.guiScale)
			{
				bounds.y = initialBounds.y + initialBounds.height - instance.windowMinHeight * instance.guiScale;
				bounds.height = instance.windowMinHeight * instance.guiScale;
			}

			bounds.x = initialBounds.x + GetMousePosition().x - initialMousePosition.x;
			bounds.width = initialBounds.width + initialMousePosition.x - GetMousePosition().x;
			if (bounds.width < instance.windowMinWidth * instance.guiScale)
			{
				bounds.x = initialBounds.x + initialBounds.width - instance.windowMinWidth * instance.guiScale;
				bounds.width = instance.windowMinWidth * instance.guiScale;
			}
			break;

		case WindowState::ResizingBottomLeft:
			bounds.height = initialBounds.height - initialMousePosition.y + GetMousePosition().y;
			if (bounds.height < instance.windowMinHeight * instance.guiScale)
				bounds.height = instance.windowMinHeight * instance.guiScale;

			bounds.x = initialBounds.x + GetMousePosition().x - initialMousePosition.x;
			bounds.width = initialBounds.width + initialMousePosition.x - GetMousePosition().x;
			if (bounds.width < instance.windowMinWidth * instance.guiScale)
			{
				bounds.x = initialBounds.x + initialBounds.width - instance.windowMinWidth * instance.guiScale;
				bounds.width = instance.windowMinWidth * instance.guiScale;
			}
			break;

		case WindowState::ResizingTopRight:
			bounds.y = initialBounds.y + GetMousePosition().y - initialMousePosition.y;
			bounds.height = initialBounds.height + initialMousePosition.y - GetMousePosition().y;
			if (bounds.height < instance.windowMinHeight * instance.guiScale)
			{
				bounds.y = initialBounds.y + initialBounds.height - instance.windowMinHeight * instance.guiScale;
				bounds.height = instance.windowMinHeight * instance.guiScale;
			}

			bounds.width = initialBounds.width - initialMousePosition.x + GetMousePosition().x;
			if (bounds.width < instance.windowMinWidth * instance.guiScale)
				bounds.width = instance.windowMinWidth * instance.guiScale;
			break;

		case WindowState::ResizingBottomRight:
			bounds.height = initialBounds.height - initialMousePosition.y + GetMousePosition().y;
			if (bounds.height < instance.windowMinHeight * instance.guiScale)
				bounds.height = instance.windowMinHeight * instance.guiScale;

			bounds.width = initialBounds.width - initialMousePosition.x + GetMousePosition().x;
			if (bounds.width < instance.windowMinWidth * instance.guiScale)
				bounds.width = instance.windowMinWidth * instance.guiScale;
			break;
		}

		if (bounds.width < instance.windowMinWidth * instance.guiScale)
		{
			bounds.x = bounds.x - (bounds.width / (2.0f * instance.guiScale));
			bounds.width = instance.windowMinWidth * instance.guiScale;
		}
		if (bounds.height < instance.windowMinHeight * instance.guiScale)
		{
			bounds.y = bounds.y - (bounds.height / (2.0f * instance.guiScale));
			bounds.height = instance.windowMinHeight * instance.guiScale;
		} /*

		 float borderThickness = std::round(instance.borderSize * instance.guiScale);

		 widgits[0]->bounds = {borderThickness, borderThickness, bounds.width - borderThickness * 2.0f, bounds.height - borderThickness * 2.0f};
		 if (config[int(WindowConfiguration::TitleBar)])
			 // widgits[1]->bounds = {borderThickness, borderThickness, bounds.width - borderThickness * 2.0f, (instance.guiScale - borderThickness) * 2.0f};
			 if (config[int(WindowConfiguration::MaximizeButton)])
				 // widgits[2]->bounds = {borderThickness, borderThickness, bounds.width - borderThickness * 2.0f, bounds.height - borderThickness * 2.0f};
				 if (config[int(WindowConfiguration::MinimizeButton)])
					 // widgits[3]->bounds = {borderThickness, borderThickness, bounds.width - borderThickness * 2.0f, bounds.height - borderThickness * 2.0f};
					 if (config[int(WindowConfiguration::CloseButton)])
						 // widgits[4]->bounds = {borderThickness, borderThickness, bounds.width - borderThickness * 2.0f, bounds.height - borderThickness * 2.0f};
						 if (config[int(WindowConfiguration::OptionsButton)])
							 // widgits[5]->bounds = {borderThickness, borderThickness, bounds.width - borderThickness * 2.0f, bounds.height - borderThickness * 2.0f};
							 if (config[int(WindowConfiguration::InfoButton)])
								 // widgits[6]->bounds = {borderThickness, borderThickness, bounds.width - borderThickness * 2.0f, bounds.height - borderThickness * 2.0f};
								 ;*/
	}

	for (int index = 0; index < widgits.size(); index++)
		widgits[index]->update();

	if (instance.redrawing)
		requireRedrawing = true;
}

void rde::Instance::Window::draw(bool a_showDebugBounds, bool a_forceRedrawAll)
{
	if (!(instance.useShader && (a_forceRedrawAll || requireRedrawing)))
		return;

	updateConfigBounds();
	float thickness = std::round(instance.thickness * instance.guiScale) + 1.0f;
	float roundness = instance.roundness * instance.guiScale;

	// Blur the first background and put it to second background
	if (a_forceRedrawAll)
	{
		BeginTextureMode(instance.backgroundTwo);
		{
			BeginShaderMode(instance.blurShader);
			{
				DrawTexturePro(instance.backgroundOne.texture, {0, 0, float(instance.backgroundOne.texture.width), float(-instance.backgroundOne.texture.height)}, {0, 0, instance.resolution[0], instance.resolution[1]}, {0}, 0, WHITE);
			}
			EndShaderMode();
		}
		EndTextureMode();
	}

	// Use the blurred background and draw it with window in first background
	if (requireRedrawing)
	{
		BeginTextureMode(instance.backgroundOne);
		{
			instance.rectangle[0] = bounds.x;
			instance.rectangle[1] = instance.resolution[1] - bounds.y - bounds.height;
			instance.rectangle[2] = bounds.width;
			instance.rectangle[3] = bounds.height;
			SetShaderValue(instance.shadowShader, GetShaderLocation(instance.shadowShader, "u_rectangle"), instance.rectangle, SHADER_UNIFORM_VEC4);

			BeginShaderMode(instance.shadowShader);
			{
				DrawTexturePro(instance.backgroundTwo.texture, Rectangle{0, 0, float(instance.backgroundTwo.texture.width), float(-instance.backgroundTwo.texture.height)}, {0, 0, instance.resolution[0], instance.resolution[1]}, {0}, 0, WHITE);
			}
			EndShaderMode();

			requireRedrawing = false;

			for (int index = 0; index < widgits.size(); index++)
				widgits[index]->draw();

			if (a_showDebugBounds)
				drawDebugBounds();
		}
		EndTextureMode();
	}
}

void rde::Instance::Window::updateConfigBounds()
{
	float halfGuiScale = instance.guiScale * 0.5f;

	// Inside window
	configBounds[0] = {bounds.x - instance.guiScale, bounds.y - instance.guiScale, bounds.width + instance.guiScale, bounds.height + instance.guiScale};
	// Top Edge
	configBounds[1] = {bounds.x + instance.guiScale * instance.roundness, bounds.y - halfGuiScale, bounds.width - instance.guiScale * instance.roundness * 2.0f, instance.guiScale};
	// Bottom Edge
	configBounds[2] = {bounds.x + instance.guiScale * instance.roundness, bounds.y + bounds.height - halfGuiScale, bounds.width - instance.guiScale * instance.roundness * 2.0f, instance.guiScale};
	// Left Edge
	configBounds[3] = {bounds.x - halfGuiScale, bounds.y + instance.guiScale * instance.roundness, instance.guiScale, bounds.height - instance.guiScale * instance.roundness * 2.0f};
	// Right Edge
	configBounds[4] = {bounds.x + bounds.width - halfGuiScale, bounds.y + instance.guiScale * instance.roundness, instance.guiScale, bounds.height - instance.guiScale * instance.roundness * 2.0f};
	// Top Left Corner
	configBounds[5] = {bounds.x - halfGuiScale, bounds.y - halfGuiScale, instance.guiScale, instance.guiScale};
	// Bottom Left Corner
	configBounds[6] = {bounds.x - halfGuiScale, bounds.y + bounds.height - halfGuiScale, instance.guiScale, instance.guiScale};
	// Top Right Corner
	configBounds[7] = {bounds.x + bounds.width - halfGuiScale, bounds.y - halfGuiScale, instance.guiScale, instance.guiScale};
	// Bottom Right Corner
	configBounds[8] = {bounds.x + bounds.width - halfGuiScale, bounds.y + bounds.height - halfGuiScale, instance.guiScale, instance.guiScale};

	float capThickness = ((instance.roundness < 0.5f) ? 0 : instance.roundness - 0.5f) * instance.guiScale;

	// Top Left Bottom Corner
	configBounds[10] = {bounds.x - halfGuiScale, bounds.y + halfGuiScale, instance.guiScale, capThickness};
	// Top Left Right Corner
	configBounds[11] = {bounds.x + halfGuiScale, bounds.y - halfGuiScale, capThickness, instance.guiScale};
	// Bottom Left Top Corner
	configBounds[12] = {bounds.x - halfGuiScale, bounds.y + bounds.height - halfGuiScale - capThickness, instance.guiScale, capThickness};
	// Bottom Left Right Corner
	configBounds[13] = {bounds.x + halfGuiScale, bounds.y + bounds.height - halfGuiScale, capThickness, instance.guiScale};
	// Top Right Bottom Corner
	configBounds[14] = {bounds.x + bounds.width - halfGuiScale, bounds.y + halfGuiScale, instance.guiScale, capThickness};
	// Top Right Left Corner
	configBounds[15] = {bounds.x + bounds.width - halfGuiScale - capThickness, bounds.y - halfGuiScale, capThickness, instance.guiScale};
	// Bottom Right Top Corner
	configBounds[16] = {bounds.x + bounds.width - halfGuiScale, bounds.y + bounds.height - halfGuiScale - capThickness, instance.guiScale, capThickness};
	// Bottom Right Left Corner
	configBounds[17] = {bounds.x + bounds.width - halfGuiScale - capThickness, bounds.y + bounds.height - halfGuiScale, capThickness, instance.guiScale};

	// Main Bounds
	configBounds[9] = bounds;
}

void rde::Instance::Window::drawDebugBounds()
{
	// Main Bounds
	DrawRectangleLinesEx(configBounds[9], 1, GRAY);
	// Top Edge
	DrawRectangleLinesEx(configBounds[1], 1, GREEN);
	// Bottom Edge
	DrawRectangleLinesEx(configBounds[2], 1, GREEN);
	// Left Edge
	DrawRectangleLinesEx(configBounds[3], 1, GREEN);
	// Right Edge
	DrawRectangleLinesEx(configBounds[4], 1, GREEN);
	// Top Left Corner
	DrawRectangleLinesEx(configBounds[5], 1, YELLOW);
	// Bottom Left Corner
	DrawRectangleLinesEx(configBounds[6], 1, YELLOW);
	// Top Right Corner
	DrawRectangleLinesEx(configBounds[7], 1, YELLOW);
	// Bottom Right Corner
	DrawRectangleLinesEx(configBounds[8], 1, YELLOW);

	if (instance.roundness > 0.5f)
	{
		DrawRectangleLinesEx(configBounds[10], 1, ORANGE);
		DrawRectangleLinesEx(configBounds[11], 1, ORANGE);
		DrawRectangleLinesEx(configBounds[12], 1, ORANGE);
		DrawRectangleLinesEx(configBounds[13], 1, ORANGE);
		DrawRectangleLinesEx(configBounds[14], 1, ORANGE);
		DrawRectangleLinesEx(configBounds[15], 1, ORANGE);
		DrawRectangleLinesEx(configBounds[16], 1, ORANGE);
		DrawRectangleLinesEx(configBounds[17], 1, ORANGE);
	}

	// Virtual state and window state as the title text
	DrawText(TextFormat("Visual: %i, Actual: %i\nPos: %f, %f\nDim: %f, %f", int(visualState), int(windowState), bounds.x, bounds.y, bounds.width, bounds.height), bounds.x + 0.5f * instance.guiScale, bounds.y + 0.5f * instance.guiScale, 5, WHITE);
}

rde::Instance::WidgitTemplate::WidgitTemplate(Instance &a_instance, Window *a_window, Rectangle a_bounds, Color a_gradientColorOne, Color a_gradientColorTwo, Vector4 a_mulColor, float a_roundness, WidgitPosition a_widgitPosition, WidgitAnchor a_widgitAnchor)
	: instance(a_instance), window(a_window), bounds(a_bounds), gradientColorOne(a_gradientColorOne), gradientColorTwo(a_gradientColorTwo), mulColor(a_mulColor), roundness(a_roundness), widgitPosition(a_widgitPosition), widgitAnchor(a_widgitAnchor)
{
	widgitState = WidgitState::Idle;
	widgitPosition = WidgitPosition::Free;
	widgitAnchor = WidgitAnchor::None;
}

void rde::Instance::WidgitTemplate::update()
{
}

void rde::Instance::WidgitTemplate::draw()
{
	float localRoundness = roundness * instance.roundness * instance.guiScale;
	SetShaderValue(instance.widgitShader, GetShaderLocation(instance.widgitShader, "u_roundness"), &localRoundness, SHADER_UNIFORM_FLOAT);

	rectangle[0] = bounds.x;
	rectangle[1] = bounds.y;
	rectangle[2] = bounds.width;
	rectangle[3] = bounds.height;

	if (window)
	{
		rectangle[0] += window->bounds.x;
		rectangle[1] += window->bounds.y;
	}

	rectangle[1] = instance.resolution[1] - rectangle[1] - rectangle[3];

	SetShaderValue(instance.widgitShader, GetShaderLocation(instance.widgitShader, "u_rectangle"), rectangle, SHADER_UNIFORM_VEC4);

	color[0] = instance.backgroundColor.r / 255.0f * mulColor.x;
	color[1] = instance.backgroundColor.g / 255.0f * mulColor.y;
	color[2] = instance.backgroundColor.b / 255.0f * mulColor.z;
	color[3] = instance.backgroundColor.a / 255.0f * mulColor.w;
	SetShaderValue(instance.widgitShader, GetShaderLocation(instance.widgitShader, "u_color"), color, SHADER_UNIFORM_VEC4);

	BeginShaderMode(instance.widgitShader);
	{
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLANK);
	}
	EndShaderMode();

	// DrawRectangleRounded(bounds, utils::getRecRoundness(bounds, instance.roundness * (instance.guiScale * 0.5f)), 1, WHITE);
}

// if (CheckCollisionPointRec(GetMousePosition(), configBounds[1]))
// visualState = WindowState::TitleBar;
// if (CheckCollisionPointRec(GetMousePosition(), configBounds[2]) && bounds.width > 1.5f * instance.windowMinHeight * instance.guiScale)
// visualState = WindowState::CloseButton;
// if (CheckCollisionPointRec(GetMousePosition(), configBounds[3]) && bounds.width > 2.0f * instance.windowMinHeight * instance.guiScale)
// visualState = WindowState::MaxButton;
// if (CheckCollisionPointRec(GetMousePosition(), configBounds[4]) && bounds.width > 2.5f * instance.windowMinHeight * instance.guiScale)
// visualState = WindowState::MinButton;

// case WindowState::TitleBar:
// bounds.x = initialBounds.x + GetMousePosition().x - initialMousePosition.x;
// bounds.y = initialBounds.y + GetMousePosition().y - initialMousePosition.y;
// break;

// Title Bar
// configBounds[1] = {bounds.x + 0.5f * instance.guiScale, bounds.y + 0.5f * instance.guiScale, bounds.width - instance.guiScale, instance.guiScale};
// Close Button
// configBounds[2] = {bounds.x + bounds.width - 1.5f * instance.guiScale, bounds.y + 0.5f * instance.guiScale, instance.guiScale, instance.guiScale};
// Min Button
// configBounds[3] = {bounds.x + bounds.width - 2.5f * instance.guiScale, bounds.y + 0.5f * instance.guiScale, instance.guiScale, instance.guiScale};
// Max Button
// configBounds[4] = {bounds.x + bounds.width - 3.5f * instance.guiScale, bounds.y + 0.5f * instance.guiScale, instance.guiScale, instance.guiScale};

// Title Bar
// DrawRectangleLinesEx(configBounds[1], 1, RED);
// Close button
// DrawRectangleLinesEx(configBounds[2], 1, BLUE);
// Max button
// DrawRectangleLinesEx(configBounds[3], 1, BLUE);
// Min button
// DrawRectangleLinesEx(configBounds[4], 1, BLUE);

// Close Button
// if (bounds.width > 1.5f * instance.windowMinHeight * instance.guiScale)
// {
// DrawLineEx({bounds.x + bounds.width - 1.1875f * instance.guiScale, bounds.y + 0.8125f * instance.guiScale}, {bounds.x + bounds.width - 0.8125f * instance.guiScale, bounds.y + 1.1875f * instance.guiScale}, thickness, WHITE);
// DrawLineEx({bounds.x + bounds.width - 1.1875f * instance.guiScale, bounds.y + 1.1875f * instance.guiScale}, {bounds.x + bounds.width - 0.8125f * instance.guiScale, bounds.y + 0.8125f * instance.guiScale}, thickness, WHITE);
// }
// Max Button
// if (bounds.width > 2.0f * instance.windowMinHeight * instance.guiScale)
// {
// DrawRectangleLinesEx({bounds.x + bounds.width - 2.1875f * instance.guiScale - 0.5f * thickness, bounds.y + 0.8125f * instance.guiScale - 0.5f * thickness, 0.375f * instance.guiScale + thickness, 0.375f * instance.guiScale + thickness}, thickness, WHITE);
// }
// Min Button
// if (bounds.width > 2.5f * instance.windowMinHeight * instance.guiScale)
// {
// DrawLineEx({bounds.x + bounds.width - 3.1875f * instance.guiScale, bounds.y + instance.guiScale}, {bounds.x + bounds.width - 2.8125f * instance.guiScale, bounds.y + instance.guiScale}, thickness, WHITE);
// }
