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

#pragma once

#ifndef RDE_HPP
// rde.hpp
#define RDE_HPP

#include <vector>
#include <string>
#include <cmath>

// A simple and easy-to-use library to enjoy videogames programming (www.raylib.com)
#include "raylib.h"

#include "ucore.hpp"

namespace rde
{
	enum class WindowState
	{
		OutsideWindow, // Outside Window
		InsideWindow,

		Closing,
		Closed,
		Maximizing,
		Maximized,
		Minimizing,
		Minimized,

		ResizingTop,
		ResizingBottom,
		ResizingLeft,
		ResizingRight,

		ResizingTopLeft,
		ResizingBottomLeft,
		ResizingTopRight,
		ResizingBottomRight,
	};

	enum class WidgitState
	{
		Idle,
		Active,

		IdleHover,
		IdleHeld,
		ActiveHover,
		ActiveHeld
	};

	enum class WidgitPosition
	{
		Free,
		Full,

		Top,
		Bottom,
		Left,
		Right,
	};

	enum class WidgitAnchor
	{
		None,

		Top,
		Bottom,
		Left,
		Right,

		TopLeft,
		BottomLeft,
		TopRight,
		BottomRight,
	};

	enum class WindowConfiguration
	{
		TitleBar,
		CloseButton,
		MaximizeButton,
		MinimizeButton,
		OptionsButton,
		InfoButton,
		SideBar,
		MAX
	};

	class Instance
	{
		// Shader Values

		// Window Shader

		float resolution[2];
		float guiScale;

		float blurRadius;
		float blurQuality;

		float rectangle[4];
		float roundness;
		float thickness;
		float borderSize;

		float shadowRadius;
		float shadowSize;
		float shadowColor[4];
		float shadowOffset[2];

		// Blur Shader blurs out the background depending on the variables
		// Shadow Shader creates shadow on the given rectangle depending on the variables
		// Gradient Shader makes smooth transition from two colors (only two colors sadly (I couldn't figure out a good, understandable shader for 3+ colors))

		// Yep, it's actually source code of the shader..................

		std::string blurShaderSourceCode;
		std::string shadowShaderSourceCode;
		std::string gradientShaderSourceCode;
		std::string widgitShaderSourceCode;

		Shader blurShader;
		Shader shadowShader;
		Shader gradientShader;
		Shader widgitShader;

		void generateBlurShaderSourceCode();
		void generateShadowShaderSourceCode();
		void generateGradientShaderSourceCode();
		void generatewidgitShaderSourceCode();

		/// @brief
		RenderTexture backgroundOne;
		RenderTexture backgroundTwo;

		// Based on the GuiScale

		float windowMinWidth;
		float windowMinHeight;

		bool redrawing;

	public:
		class WidgitTemplate;
		class Window;

		std::vector<Window *> windows;

		bool useShader;
		Color foregroundColor = {255, 255, 255, 255};
		Color backgroundColor = {0, 0, 0, 127};

		void load(bool a_useShader = true);
		void unload();
		void update(bool a_updateResolition = true);
		void draw(bool a_showDebugBounds = false, bool a_forceRedrawAll = false);

		// Draw things in this to make it appear in the background of the window

		void beginBackground();
		void endBackground();

		// Shader getters and setters

		void setShaderResolution(Vector2 a_resolution);
		Vector2 getShaderResolution();

		float getGuiScale();
		void setGuiScale(float a_guiScale);

		float getBlurRadius();
		void setBlurRadius(float a_blurRadius);
		float getBlurQuality();

		/// @brief Requires re-compilation of the shader (handled inside the function)
		void setBlurQuality(float a_BlurQuality);

		float getRoundness();
		void setRoundness(float a_roundness);
		float getThickness();
		void setThickness(float a_thickness);
		float getBorderSize();
		void setBorderSize(float a_borderSize);

		float getShadowRadius();
		void setShadowRadius(float a_shadowRadius);
		float getShadowSize();
		void setShadowSize(float a_shadowSize);
		Color getShadowColor();
		void setShadowColor(Color a_shadowColor);
		Vector2 getShadowOffset();
		void setShadowOffset(Vector2 a_shadowOffset);

		class Window
		{
			Vector2 initialMousePosition;
			Rectangle initialBounds;

			// Array size is from:
			// Inside window, Top/Bottom/Left/Right egde, Top-Left/Bottom-Left/Top-Right/Bottom-Right corner, MainWindow, Top-Left-Bottom/Top-Left-Right/Bottom-Left-Top/Bottom-Left-Right/Top-Right-Bottom/Top-Right-Left/Bottom-Right-Top/Bottom-Right-Left
			Rectangle configBounds[18];

			Instance &instance;
			std::vector<WidgitTemplate *> widgits;
			std::vector<bool> config;
			bool requireRedrawing;

		public:
			WindowState windowState = WindowState::OutsideWindow;
			WindowState visualState = WindowState::OutsideWindow;
			Rectangle bounds;

			/// @brief Initialize window
			/// @param a_instance The target instance for the window
			/// @param a_bounds The dimentions of the window
			/// @param a_config Configurations for the window consisting of 10 elements of booleans which determine how to window is constructed
			Window(Instance &a_instance, Rectangle a_bounds, std::vector<bool> a_config = {true, true, true, true, true, true});
			~Window();
			void update();
			void draw(bool a_showDebugBounds = false, bool a_forceRedrawAll = false);
			void updateConfigBounds();
			void drawDebugBounds();
		};

		class WidgitTemplate
		{
			Instance &instance;
			// Optional
			Window *window;
			float rectangle[4];
			float roundness;
			float color[4];

		public:
			Color gradientColorOne;
			Color gradientColorTwo;
			Vector4 mulColor;
			Rectangle bounds;
			WidgitState widgitState;
			WidgitPosition widgitPosition;
			WidgitAnchor widgitAnchor;

			WidgitTemplate(Instance &a_instance, Window *a_window, Rectangle a_bounds, Color a_gradientColorOne, Color a_gradientColorTwo, Vector4 a_mulColor, float a_roundness, WidgitPosition a_widgitPosition = WidgitPosition::Free, WidgitAnchor a_widgitAnchor = WidgitAnchor::None);
			virtual void update();
			virtual void draw();

			friend class Window;
		};

		class Border : public WidgitTemplate
		{
		public:
		};

		class Button : public WidgitTemplate
		{
		public:
		};

		class Toggle : public WidgitTemplate
		{
		public:
		};

		class TextBox : public WidgitTemplate
		{
		public:
		};

		class DropBox : public WidgitTemplate
		{
		public:
		};

		class CheckBox : public WidgitTemplate
		{
		public:
		};

		class LabelBox : public WidgitTemplate
		{
		public:
		};

		class GroupBox : public WidgitTemplate
		{
		public:
		};

		class SlideBar : public WidgitTemplate
		{
		public:
		};
	};
}

// Header Guard End
#endif
