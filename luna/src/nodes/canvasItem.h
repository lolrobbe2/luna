#pragma once
#include <nodes/controlNodes/controlNode.h>
#include <core/scene/scene.h>

#ifndef PT_TO_PX
#define PT_TO_PX(Point) Point * ( 72 / 96 )
#endif // !PT_TO_PX

#ifndef LN_DRAW_LAMBDA
#define LN_DRAW_LAMBDA(nodeClass) [=]() { nodeClass(entityHandle,scene).draw(); }
#endif
namespace luna 
{
	class color
	{
	public:
		color() = default;
		color(float r, float g, float b, float a) :r(r), g(g), b(b), a(a) {};
		virtual ~color() = default;
		operator glm::vec4() { return { r,g,b,a }; };

	private:
		float r, g, b, a;
	};


	namespace nodes 
	{

		class canvasItem : public controlNode
		{
		public:
			canvasItem() = default;
			canvasItem(entt::entity handle, luna::scene* scene);
			canvasItem(uint64_t id, luna::scene* scene);
			canvasItem(luna::scene* scene);
			
			void drawChar(ref<renderer::font> font, glm::vec2 pos, char chr, int font_size = 16, color modulate = color(1, 1, 1, 1));
			void drawString(ref<renderer::font> font, glm::vec2 pos, std::string chr, int font_size = 16, color modulate = color(1, 1, 1, 1));
			void drawTexture(ref<renderer::texture> texture, glm::vec2 position, color modulate);
			void executeDraw();
			virtual void draw() { LN_CORE_INFO("canvasItem"); };
		};
	}
	namespace colors
	{
		const color ALICE_BLUE = color(0.941176, 0.972549, 1, 1);
		const color ANTIQUE_WHITE = color(0.980392, 0.921569, 0.843137, 1);
		const color AQUA = color(0, 1, 1, 1);
		const color AQUAMARINE = color(0.498039, 1, 0.831373, 1);
		const color AZURE = color(0.941176, 1, 1, 1);
		const color BEIGE = color(0.960784, 0.960784, 0.862745, 1);
		const color BISQUE = color(1, 0.894118, 0.768627, 1);
		const color BLACK = color(0, 0, 0, 1);
		const color BLANCHED_ALMOND = color(1, 0.921569, 0.803922, 1);
		const color BLUE = color(0, 0, 1, 1);
		const color BLUE_VIOLET = color(0.541176, 0.168627, 0.886275, 1);
		const color BROWN = color(0.647059, 0.164706, 0.164706, 1);
		const color BURLYWOOD = color(0.870588, 0.721569, 0.529412, 1);
		const color CADET_BLUE = color(0.372549, 0.619608, 0.627451, 1);
		const color CHARTREUSE = color(0.498039, 1, 0, 1);
		const color CHOCOLATE = color(0.823529, 0.411765, 0.117647, 1);
		const color CORAL = color(1, 0.498039, 0.313726, 1);
		const color CORNFLOWER_BLUE = color(0.392157, 0.584314, 0.929412, 1);
		const color CORNSILK = color(1, 0.972549, 0.862745, 1);
		const color CRIMSON = color(0.862745, 0.0784314, 0.235294, 1);
		const color CYAN = color(0, 1, 1, 1);
		const color DARK_BLUE = color(0, 0, 0.545098, 1);
		const color DARK_CYAN = color(0, 0.545098, 0.545098, 1);
		const color DARK_GOLDENROD = color(0.721569, 0.52549, 0.0431373, 1);
		const color DARK_GRAY = color(0.662745, 0.662745, 0.662745, 1);
		const color DARK_GREEN = color(0, 0.392157, 0, 1);
		const color DARK_KHAKI = color(0.741176, 0.717647, 0.419608, 1);
		const color DARK_MAGENTA = color(0.545098, 0, 0.545098, 1);
		const color DARK_OLIVE_GREEN = color(0.333333, 0.419608, 0.184314, 1);
		const color DARK_ORANGE = color(1, 0.54902, 0, 1);
		const color DARK_ORCHID = color(0.6, 0.196078, 0.8, 1);
		const color DARK_RED = color(0.545098, 0, 0, 1);
		const color DARK_SALMON = color(0.913725, 0.588235, 0.478431, 1);
		const color DARK_SEA_GREEN = color(0.560784, 0.737255, 0.560784, 1);
		const color DARK_SLATE_BLUE = color(0.282353, 0.239216, 0.545098, 1);
		const color DARK_SLATE_GRAY = color(0.184314, 0.309804, 0.309804, 1);
		const color DARK_TURQUOISE = color(0, 0.807843, 0.819608, 1);
		const color DARK_VIOLET = color(0.580392, 0, 0.827451, 1);
		const color DEEP_PINK = color(1, 0.0784314, 0.576471, 1);
		const color DEEP_SKY_BLUE = color(0, 0.74902, 1, 1);
		const color DIM_GRAY = color(0.411765, 0.411765, 0.411765, 1);
		const color DODGER_BLUE = color(0.117647, 0.564706, 1, 1);
		const color FIREBRICK = color(0.698039, 0.133333, 0.133333, 1);
		const color FLORAL_WHITE = color(1, 0.980392, 0.941176, 1);
		const color FOREST_GREEN = color(0.133333, 0.545098, 0.133333, 1);
		const color FUCHSIA = color(1, 0, 1, 1);
		const color GAINSBORO = color(0.862745, 0.862745, 0.862745, 1);
		const color GHOST_WHITE = color(0.972549, 0.972549, 1, 1);
		const color GOLD = color(1, 0.843137, 0, 1);
		const color GOLDENROD = color(0.854902, 0.647059, 0.12549, 1);
		const color GRAY = color(0.745098, 0.745098, 0.745098, 1);
		const color GREEN = color(0, 1, 0, 1);
		const color GREEN_YELLOW = color(0.678431, 1, 0.184314, 1);
		const color HONEYDEW = color(0.941176, 1, 0.941176, 1);
		const color HOT_PINK = color(1, 0.411765, 0.705882, 1);
		const color INDIAN_RED = color(0.803922, 0.360784, 0.360784, 1);
		const color INDIGO = color(0.294118, 0, 0.509804, 1);
		const color IVORY = color(1, 1, 0.941176, 1);
		const color KHAKI = color(0.941176, 0.901961, 0.54902, 1);
		const color LAVENDER = color(0.901961, 0.901961, 0.980392, 1);
		const color LAVENDER_BLUSH = color(1, 0.941176, 0.960784, 1);
		const color LAWN_GREEN = color(0.486275, 0.988235, 0, 1);
		const color LEMON_CHIFFON = color(1, 0.980392, 0.803922, 1);
		const color LIGHT_BLUE = color(0.678431, 0.847059, 0.901961, 1);
		const color LIGHT_CORAL = color(0.941176, 0.501961, 0.501961, 1);
		const color LIGHT_CYAN = color(0.878431, 1, 1, 1);
		const color LIGHT_GOLDENROD = color(0.980392, 0.980392, 0.823529, 1);
		const color LIGHT_GRAY = color(0.827451, 0.827451, 0.827451, 1);
		const color LIGHT_GREEN = color(0.564706, 0.933333, 0.564706, 1);
		const color LIGHT_PINK = color(1, 0.713726, 0.756863, 1);
		const color LIGHT_SALMON = color(1, 0.627451, 0.478431, 1);
		const color LIGHT_SEA_GREEN = color(0.12549, 0.698039, 0.666667, 1);
		const color LIGHT_SKY_BLUE = color(0.529412, 0.807843, 0.980392, 1);
		const color LIGHT_SLATE_GRAY = color(0.466667, 0.533333, 0.6, 1);
		const color LIGHT_STEEL_BLUE = color(0.690196, 0.768627, 0.870588, 1);
		const color LIGHT_YELLOW = color(1, 1, 0.878431, 1);
		const color LIME = color(0, 1, 0, 1);
		const color LIME_GREEN = color(0.196078, 0.803922, 0.196078, 1);
		const color LINEN = color(0.980392, 0.941176, 0.901961, 1);
		const color MAGENTA = color(1, 0, 1, 1);
		const color MAROON = color(0.690196, 0.188235, 0.376471, 1);
		const color MEDIUM_AQUAMARINE = color(0.4, 0.803922, 0.666667, 1);
		const color MEDIUM_BLUE = color(0, 0, 0.803922, 1);
		const color MEDIUM_ORCHID = color(0.729412, 0.333333, 0.827451, 1);
		const color MEDIUM_PURPLE = color(0.576471, 0.439216, 0.858824, 1);
		const color MEDIUM_SEA_GREEN = color(0.235294, 0.701961, 0.443137, 1);
		const color MEDIUM_SLATE_BLUE = color(0.482353, 0.407843, 0.933333, 1);
		const color MEDIUM_SPRING_GREEN = color(0, 0.980392, 0.603922, 1);
		const color MEDIUM_TURQUOISE = color(0.282353, 0.819608, 0.8, 1);
		const color MEDIUM_VIOLET_RED = color(0.780392, 0.0823529, 0.521569, 1);
		const color MIDNIGHT_BLUE = color(0.0980392, 0.0980392, 0.439216, 1);
		const color MINT_CREAM = color(0.960784, 1, 0.980392, 1);
		const color MISTY_ROSE = color(1, 0.894118, 0.882353, 1);
		const color MOCCASIN = color(1, 0.894118, 0.709804, 1);
		const color NAVAJO_WHITE = color(1, 0.870588, 0.678431, 1);
		const color NAVY_BLUE = color(0, 0, 0.501961, 1);
		const color OLD_LACE = color(0.992157, 0.960784, 0.901961, 1);
		const color OLIVE = color(0.501961, 0.501961, 0, 1);
		const color OLIVE_DRAB = color(0.419608, 0.556863, 0.137255, 1);
		const color ORANGE = color(1, 0.647059, 0, 1);
		const color ORANGE_RED = color(1, 0.270588, 0, 1);
		const color ORCHID = color(0.854902, 0.439216, 0.839216, 1);
		const color PALE_GOLDENROD = color(0.933333, 0.909804, 0.666667, 1);
		const color PALE_GREEN = color(0.596078, 0.984314, 0.596078, 1);
		const color PALE_TURQUOISE = color(0.686275, 0.933333, 0.933333, 1);
		const color PALE_VIOLET_RED = color(0.858824, 0.439216, 0.576471, 1);
		const color PAPAYA_WHIP = color(1, 0.937255, 0.835294, 1);
		const color PEACH_PUFF = color(1, 0.854902, 0.72549, 1);
		const color PERU = color(0.803922, 0.521569, 0.247059, 1);
		const color PINK = color(1, 0.752941, 0.796078, 1);
		const color PLUM = color(0.866667, 0.627451, 0.866667, 1);
		const color POWDER_BLUE = color(0.690196, 0.878431, 0.901961, 1);
		const color PURPLE = color(0.627451, 0.12549, 0.941176, 1);
		const color REBECCA_PURPLE = color(0.4, 0.2, 0.6, 1);
		const color RED = color(1, 0, 0, 1);
		const color ROSY_BROWN = color(0.737255, 0.560784, 0.560784, 1);
		const color ROYAL_BLUE = color(0.254902, 0.411765, 0.882353, 1);
		const color SADDLE_BROWN = color(0.545098, 0.270588, 0.0745098, 1);
		const color SALMON = color(0.980392, 0.501961, 0.447059, 1);
		const color SANDY_BROWN = color(0.956863, 0.643137, 0.376471, 1);
		const color SEA_GREEN = color(0.180392, 0.545098, 0.341176, 1);
		const color SEASHELL = color(1, 0.960784, 0.933333, 1);
		const color SIENNA = color(0.627451, 0.321569, 0.176471, 1);
		const color SILVER = color(0.752941, 0.752941, 0.752941, 1);
		const color SKY_BLUE = color(0.529412, 0.807843, 0.921569, 1);
		const color SLATE_BLUE = color(0.415686, 0.352941, 0.803922, 1);
		const color SLATE_GRAY = color(0.439216, 0.501961, 0.564706, 1);
		const color SNOW = color(1, 0.980392, 0.980392, 1);
		const color SPRING_GREEN = color(0, 1, 0.498039, 1);
		const color STEEL_BLUE = color(0.27451, 0.509804, 0.705882, 1);
		const color TAN = color(0.823529, 0.705882, 0.54902, 1);
		const color TEAL = color(0, 0.501961, 0.501961, 1);
		const color THISTLE = color(0.847059, 0.74902, 0.847059, 1);
		const color TOMATO = color(1, 0.388235, 0.278431, 1);
		const color LN_TRANSPARENT = color(1, 1, 1, 0);
		const color TURQUOISE = color(0.25098, 0.878431, 0.815686, 1);
		const color VIOLET = color(0.933333, 0.509804, 0.933333, 1);
		const color WEB_GRAY = color(0.501961, 0.501961, 0.501961, 1);
		const color WEB_GREEN = color(0, 0.501961, 0, 1);
		const color WEB_MAROON = color(0.501961, 0, 0, 1);
		const color WEB_PURPLE = color(0.501961, 0, 0.501961, 1);
		const color WHEAT = color(0.960784, 0.870588, 0.701961, 1);
		const color WHITE = color(1, 1, 1, 1);
		const color WHITE_SMOKE = color(0.960784, 0.960784, 0.960784, 1);
		const color YELLOW = color(1, 1, 0, 1);
		const color YELLOW_GREEN = color(0.603922, 0.803922, 0.196078, 1);
	}
}


