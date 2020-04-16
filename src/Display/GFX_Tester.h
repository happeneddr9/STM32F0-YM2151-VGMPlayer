/**
 * @Name    : GFX_Tester.h
 * @Date    : 2020年2月18日 下午5:55:45
 * @Author  : happe
 * @brief   : test your display
 *
 *   __  __        __  __     _    _____       _ _
 *  |  \/  |      / _|/ _|   | |  / ____|     (_) |
 *  | \  / |_   _| |_| |_ ___| |_| (___   __ _ _| | _____  _   _
 *  | |\/| | | | |  _|  _/ _ \ __|\___ \ / _` | | |/ / _ \| | | |
 *  | |  | | |_| | | | ||  __/ |_ ____) | (_| | |   < (_) | |_| |
 *  |_|  |_|\__,_|_| |_| \___|\__|_____/ \__,_|_|_|\_\___/ \__,_|
 *
 *
 *
 */

#ifndef DISPLAY_GFX_TESTER_H_
#define DISPLAY_GFX_TESTER_H_

#include "Graphic.h"

class GFX_Tester {
	public:
		GFX_Tester(Adafruit_GFX& dis, uint8_t rotation)
			: display(dis) {
			display.setTextSize(1);
			display.setTextColor(Pixel::Green, Pixel::Black);
			display.setRotation(3);
			display.fillScreen(Pixel::Black);
			display.setCursor(0, 0);

			base_rotation = rotation;
			base_time = run_timer = obj_time = 0;
			screen_w = display.width();
			screen_h = display.height();
			bench_name = bench_info = nullptr;
			randomSeed(millis());
		}
		~GFX_Tester() = default;

		void run_testbench(void) {
			display.setRotation(base_rotation);
			text_bench();
			GFXline_bench();
			HSV2RGB_Demo();
			Gradient_Demo_bench();
			colorline_bench();

			/* Rect Demo */
			color_drawRect_bench();
			color_fillRect_bench();

			/* Circle Demo */
			color_drawCircle_bench();
			color_fillCircle_bench();

			/* Ellipse Demo */
			color_drawEllipse_bench();
			color_fillEllipse_bench();

			/* Triangle Demo */
			color_drawTriangle_bench();
			color_fillTriangle_bench();

			clearScreen_bench();

			display.setCursor(0, 0);
			display.fillScreen(Pixel::Black);
			display.print("Test Done!");
			delay(1000);
			display.setCursor(0, 0);
			display.fillScreen(Pixel::Black);
		}

		void demo() {
			GFXline_bench();
			HSV2RGB_Demo();
		}

		void HSV2RGB_Demo() {
			bench_name = "HSV2RGB Demo";
			bench_info = "";
			display.fillScreen(Pixel::Black);
			display.setCursor(0, 0);
			display.cp437(true);
			for (uint16_t i = 0; i < 256; i++) {
				display.setTextColor(Pixel::HSV2RGB(0, 255, 255 - (i >> 1)));
				if (i == '\n')
					i = ' ';
				display.write(static_cast<uint8_t>(i));
				delay(5);
			}
			delay(3000);
			display.fillScreen(Pixel::Black);
			display.setTextColor(Pixel::White);
			display.setCursor(0, 0);
			display.print("HSV to RGB");
			delay(1000);
			display.fillScreen(Pixel::Black);

			for (uint16_t i = 0; i < screen_w; i++) {
				display.drawFastVLine(i, 0, 27, Pixel::HSV2RGB(0, 255, 255 - i));
				display.drawFastVLine(i, 54, 27, Pixel::HSV2RGB(0, 255 - i));
				display.drawFastVLine(i, 108, 27, Pixel::HSV2RGB(255 - i));
				delay(10);
			}
			delay(3000);

			print_report("It just a Color Demo with ");
		}

		void colorline_bench(void) {
			bench_name = "Color Line Bench";
			bench_info = " Line";
			display.fillScreen(Pixel::Black);

			base_time = millis();
			obj_time = 0;
			while (millis() - base_time < 2000) {
				display.drawLine(random(0, screen_w), random(0, screen_h),
								 random(0, screen_w), random(0, screen_h),
								 rand() & 0xFFFF);
				obj_time++;
			}
			run_timer = millis() - base_time;

			print_report();
		}

		void GFXline_bench(void) {
			bench_name = "GFX Line Bench";
			bench_info = "";
			display.fillScreen(Pixel::Black);

			base_time = millis();
			obj_time = 0;
			int i;

			for (i = 0; i < screen_w; i += 6) {
				display.drawLine(0, 0, i, screen_h - 1, Pixel::White);
			}
			for (i = 0; i < screen_h; i += 6) {
				display.drawLine(0, 0, screen_w - 1, i, Pixel::White);
			}

			display.fillScreen(Pixel::Black);

			for (i = 0; i < screen_w; i += 6) {
				display.drawLine(0, screen_h - 1, i, 0, Pixel::White);
			}
			for (i = screen_h - 1; i >= 0; i -= 6) {
				display.drawLine(0, screen_h - 1, screen_w - 1, i,
						Pixel::White);
			}

			display.fillScreen(Pixel::Black);

			for (i = screen_w - 1; i >= 0; i -= 6) {
				display.drawLine(screen_w - 1, screen_h - 1, i, 0,
						Pixel::White);
			}
			for (i = screen_h - 1; i >= 0; i -= 6) {
				display.drawLine(screen_w - 1, screen_h - 1, 0, i,
						Pixel::White);
			}

			display.fillScreen(Pixel::Black);

			for (i = 0; i < screen_h; i += 6) {
				display.drawLine(screen_w - 1, 0, 0, i, Pixel::White);
			}
			for (i = 0; i < screen_w; i += 6) {
				display.drawLine(screen_w - 1, 0, i, screen_h - 1,
						Pixel::White);
			}

			run_timer = millis() - base_time;

			print_report();
		}

		void color_drawRect_bench(void) {
			bench_name = "Color Draw Rect Bench";
			bench_info = " Rect";
			display.fillScreen(Pixel::Black);

			base_time = millis();
			obj_time = 0;
			while (millis() - base_time < 2000) {
				display.drawRect(random(0, screen_w), random(0, screen_h),
								 rand() & 0x1F, rand() & 0x1F,
								 rand() & 0xFFFF);
				obj_time++;
			}
			run_timer = millis() - base_time;

			print_report();
		}

		void color_fillRect_bench(void){
			bench_name = "Color Fill Rect Bench";
			bench_info = " fill Rect";
			display.fillScreen(Pixel::Black);

			base_time = millis();
			obj_time = 0;
			while (millis() - base_time < 2000) {
				display.fillRect(random(0, screen_w), random(0, screen_h),
								 rand() & 0x1F, rand() & 0x1F,
								 rand() & 0xFFFF);
				obj_time++;
			}
			run_timer = millis() - base_time;

			print_report();
		}

		void color_drawCircle_bench(void) {
			bench_name = "Color Draw Circle Bench";
			bench_info = " Circle";
			display.fillScreen(Pixel::Black);

			base_time = millis();
			obj_time = 0;
			while (millis() - base_time < 2000) {
				display.drawCircle(random(0, screen_w), random(0, screen_h),
								   rand() & 0x1F, rand() & 0xFFFF);
				obj_time++;
			}
			run_timer = millis() - base_time;

			print_report();
		}

		void color_fillCircle_bench(void) {
			bench_name = "Color Fill Circle Bench";
			bench_info = " fill Circle";
			display.fillScreen(Pixel::Black);

			base_time = millis();
			obj_time = 0;
			while (millis() - base_time < 2000) {
				display.fillCircle(random(0, screen_w), random(0, screen_h),
								   rand() & 0x1F, rand() & 0xFFFF);
				obj_time++;
			}
			run_timer = millis() - base_time;

			print_report();
		}

		void color_drawEllipse_bench(void) {
			bench_name = "Color Draw Ellipse Bench";
			bench_info = " Ellipse";
			display.fillScreen(Pixel::Black);

			base_time = millis();
			obj_time = 0;
			while (millis() - base_time < 2000) {
				display.drawEllipse(random(0, screen_w), random(0, screen_h),
									rand() & 0x1F, rand() & 0x1F, rand() & 0xFFFF);
				obj_time++;
			}
			run_timer = millis() - base_time;

			print_report();
		}

		void color_fillEllipse_bench(void) {
			bench_name = "Color Fill Ellipse Bench";
			bench_info = " fill Ellipse";
			display.fillScreen(Pixel::Black);

			base_time = millis();
			obj_time = 0;
			while (millis() - base_time < 2000) {
				display.fillEllipse(random(0, screen_w), random(0, screen_h),
									rand() & 0x1F, rand() & 0x1F, rand() & 0xFFFF);
				obj_time++;
			}
			run_timer = millis() - base_time;

			print_report();
		}

		void color_drawTriangle_bench(void) {
			bench_name = "Color Draw Triangle Bench";
			bench_info = " Triangle";
			display.fillScreen(Pixel::Black);

			base_time = millis();
			obj_time = 0;
			while (millis() - base_time < 2000) {
				display.drawTriangle(random(0, screen_w), random(0, screen_h),
									 random(0, screen_w), random(0, screen_h),
									 random(0, screen_w), random(0, screen_h),
									 rand() & 0xFFFF);
				obj_time++;
			}
			run_timer = millis() - base_time;

			print_report();
		}

		void color_fillTriangle_bench(void) {
			bench_name = "Color Fill Triangle Bench";
			bench_info = " fill Triangle";
			display.fillScreen(Pixel::Black);

			base_time = millis();
			obj_time = 0;
			while (millis() - base_time < 3000) {
				display.fillTriangle(random(0, screen_w), random(0, screen_h),
									 random(0, screen_w), random(0, screen_h),
									 random(0, screen_w), random(0, screen_h),
									 rand() & 0xFFFF);
				obj_time++;
			}
			run_timer = millis() - base_time;

			print_report();
		}

		void clearScreen_bench(){
			bench_name = "Fill Screen Bench";
			bench_info = "";
			display.fillScreen(Pixel::Black);
			display << "Ready!";

			base_time = millis();
			obj_time = 0;

			for (uint32_t i = 0; i < 10; i++) {
				uint16_t bgcolor = rand() & 0xFFFF;
				for (uint32_t j = 0; j < 10; j++) {
					display.fillScreen(bgcolor);
					display.fillScreen(Pixel::Black);
				}
			}

			run_timer = millis() - base_time;
			print_report("Fill Screen 200 times in ");
		}

		void text_bench(void) {
			bench_name = "text String Bench";
			bench_info = "";
			display.fillScreen(Pixel::Black);

			const char * str = "MuffetSaikou!";
			int16_t x1, y1;
			uint16_t w1, h1;
			int8_t x_inc = (rand() & 0x01) ? 2 : -2;
			int8_t y_inc = (rand() & 0x01) ? 2 : -2;

			display.getTextBounds(str, random(screen_w - 80), random(screen_h - 10), &x1, &y1, &w1, &h1);
			display.drawRect(x1, y1, w1, h1, Pixel::White);
			display.setCursor(x1, y1);
			delay(500);
			display.fillScreen(Pixel::Black);

			uint16_t x_range = screen_w - w1 - 1;
			uint16_t y_range = screen_h - h1 - 1;

			x1 = y1 = 0;
			int16_t x2 = 0, y2 = 0;
			base_time = millis();
			obj_time = 0;

			for (uint32_t i = 0; i < 200; i++) {

				display.setTextColor(Pixel::HSV2RGB(random(256)));
				delay(10);
				display.setCursor(x1, y1);
				display.fillRect(x2, y2, w1, h1, Pixel::Black);
				display.print(str);
				x2 = x1;
				y2 = y1;

				x1 += x_inc;
				y1 += y_inc;

				if(x1 >= x_range) {
					x_inc = -2;
				} else if (x1 < 0) {
					x_inc = 2;
				}

				if (y1 >= y_range) {
					y_inc = -2;
				} else if (y1 < 0) {
					y_inc = 2;
				}
			}

			run_timer = millis() - base_time;
			print_report("Move String 200 times in ");
		}

		void Gradient_Demo_bench(){
			bench_name = "Gradient Rect";
			bench_info = "";
			display.fillScreen(Pixel::Black);

			uint8_t color_value;
			uint8_t color_hue;
			uint16_t color_now;
			uint16_t half_screen = screen_h / 2;

			base_time = millis();
			obj_time = 0;
			for (size_t j = 0; j < 10; j++) {
				color_hue = random(256);
				color_value = 255;
				for (size_t i = 0; i < screen_w; i++) {
					color_now = Pixel::HSV2RGB(color_hue, 255, color_value);
					display.drawFastVLine(i, 0, half_screen - 1, color_now);
					display.drawFastVLine(screen_w - i - 1, half_screen, half_screen, color_now);
					if(color_value != 0)
						color_value--;
				}
				display.fillScreen(Pixel::Black);
			}
			run_timer = millis() - base_time;

			print_report("10 times gradient graphic in ");
		}

		void print_report(const char * extra = "") {

			display.setCursor(0, 0);
			display.setTextColor(Pixel::White);
			display.setTextSize(1);
			display.setRotation(base_rotation);
			display.fillScreen(Pixel::Black);
			display.println(bench_name);
			display.println();
			if(obj_time)
				display << "draw " << obj_time << bench_info << " in " << run_timer << " ms";
			else
				display << extra << run_timer << " ms";
			delay(3000);
		}


	private:

	protected:
		Adafruit_GFX& display;
		uint8_t base_rotation;
		uint32_t base_time;
		uint32_t run_timer;
		uint32_t obj_time;
		uint16_t screen_w, screen_h;
		const char * bench_name;
		const char * bench_info;
};



#endif /* DISPLAY_GFX_TESTER_H_ */
