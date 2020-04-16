/*
 * Complex_GFX.cpp
 *
 *  Created on: 2020年3月24日
 *      Author: user
 */

#include "Complex_GFX.h"

Complex_GFX::Complex_GFX(Adafruit_GFX * display) :
		display(display) {

}

Complex_GFX::Complex_GFX(Adafruit_GFX& display) :
		display(&display) {

}



/***************************************************************************************
 ** Function name:           drawQuadraticBezier
 ** Description:             Draw a bezier curve between points
 ***************************************************************************************/
// Plot any quadratic Bezier curve, no restrictions on point32_t positions
// See source code http://members.chello.at/~easyfilter/bresenham.c
void Complex_GFX::drawBezier(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
		int32_t x2, int32_t y2, uint16_t color) {
	int32_t x = x0 - x1, y = y0 - y1;
	FPoint t = x0 - 2 * x1 + x2, r;

	if (x * (x2 - x1) > 0) {
		if (y * (y2 - y1) > 0)
			if (fabs((y0 - 2 * y1 + y2) / t * x) > abs(y)) {
				x0 = x2;
				x2 = x + x1;
				y0 = y2;
				y2 = y + y1;
			}
		t = (x0 - x1) / t;
		r = (1 - t) * ((1 - t) * y0 + 2.0f * t * y1) + t * t * y2;
		t = (x0 * x2 - x1 * x1) * t / (x0 - x1);
		x = floor(t + 0.5f);
		y = floor(r + 0.5f);
		r = (y1 - y0) * (t - x0) / (x1 - x0) + y0;
		drawBezierSegment(x0, y0, x, floor(r + 0.5f), x, y, color);
		r = (y1 - y2) * (t - x2) / (x1 - x2) + y2;
		x0 = x1 = x;
		y0 = y;
		y1 = floor(r + 0.5f);
	}
	if ((y0 - y1) * (y2 - y1) > 0) {
		t = y0 - 2 * y1 + y2;
		t = (y0 - y1) / t;
		r = (1 - t) * ((1 - t) * x0 + 2.0f * t * x1) + t * t * x2;
		t = (y0 * y2 - y1 * y1) * t / (y0 - y1);
		x = floor(r + 0.5f);
		y = floor(t + 0.5f);
		r = (x1 - x0) * (t - y0) / (y1 - y0) + x0;
		drawBezierSegment(x0, y0, floor(r + 0.5f), y, x, y, color);
		r = (x1 - x2) * (t - y2) / (y1 - y2) + x2;
		x0 = x;
		x1 = floor(r + 0.5f);
		y0 = y1 = y;
	}
	drawBezierSegment(x0, y0, x1, y1, x2, y2, color);
}

/***************************************************************************************
 ** Function name:           drawBezierSegment
 ** Description:             Draw a bezier segment curve between points
 ***************************************************************************************/

//  x0, y0 defines p0 etc.
//  coordinates for p0-p3 must be sequentially increasing or decreasing so
//  n0 <= n1 <= n2 or n0 >= n1 >= n2 where n is x or y, e.g.
//
//         p1 x           .x p2      p2 x.
//                   .                       .     x p1
//               .                               .
//            .                                     .
//          .                                         .
//        .                                             .
//      .                                                 .
//  p0 x                                                   x p0
//
void Complex_GFX::drawBezierSegment(int32_t x0, int32_t y0, int32_t x1,
		int32_t y1, int32_t x2, int32_t y2, uint16_t color) {
	// Check if coordinates are sequential (replaces assert)
	if (((x2 >= x1 && x1 >= x0) || (x2 <= x1 && x1 <= x0))
			&& ((y2 >= y1 && y1 >= y0) || (y2 <= y1 && y1 <= y0))) {
		// Coordinates are sequential
		int32_t sx = x2 - x1, sy = y2 - y1;
		int32_t xx = x0 - x1, yy = y0 - y1, xy;
		FPoint dx, dy, err, cur = xx * sy - yy * sx;

		if (sx * (int32_t) sx + sy * (int32_t) sy > xx * xx + yy * yy) {
			x2 = x0;
			x0 = sx + x1;
			y2 = y0;
			y0 = sy + y1;
			cur = -cur;
		}
		display->startWrite();
		if (cur != 0) {
			xx += sx;
			xx *= sx = x0 < x2 ? 1 : -1;
			yy += sy;
			yy *= sy = y0 < y2 ? 1 : -1;
			xy = 2 * xx * yy;
			xx *= xx;
			yy *= yy;
			if (cur * sx * sy < 0) {
				xx = -xx;
				yy = -yy;
				xy = -xy;
				cur = -cur;
			}
			dx = 4.0f * sy * cur * (x1 - x0) + xx - xy;
			dy = 4.0f * sx * cur * (y0 - y1) + yy - xy;
			xx += xx;
			yy += yy;
			err = dx + dy + xy;
			do {
				display->drawPixel(x0, y0, color);
				if (x0 == x2 && y0 == y2) {
					display->endWrite();
					return;
				}
				y1 = 2 * err < dx;
				if (2 * err > dy) {
					x0 += sx;
					dx -= xy;
					err += dy += yy;
				}
				if (y1) {
					y0 += sy;
					dy -= xy;
					err += dx += xx;
				}
				yield();
			} while (dy < dx);
		}
		display->drawLine(x0, y0, x2, y2, color);
		display->endWrite();
	}
}



void Complex_GFX::drawCubicBezierSeg(int32_t x0, int32_t y0, FPoint x1, FPoint y1, FPoint x2, FPoint y2,
		int32_t x3, int32_t y3, uint16_t color) { /* plot limited cubic Bezier segment */
	int32_t f, fx, fy, leg = 1;
	int32_t sx = x0 < x3 ? 1 : -1, sy = y0 < y3 ? 1 : -1; /* step direction */
	FPoint xc = -fabs(x0 + x1 - x2 - x3), xa = xc - 4 * sx * (x1 - x2), xb = sx
			* (x0 - x1 - x2 + x3);
	FPoint yc = -fabs(y0 + y1 - y2 - y3), ya = yc - 4 * sy * (y1 - y2), yb = sy
			* (y0 - y1 - y2 + y3);
	FPoint ab, ac, bc, cb, xx, xy, yy, dx, dy, ex, *pxy, EP = 0.01f;
	/* check for curve restrains */
	/* slope P0-P1 == P2-P3    and  (P0-P3 == P1-P2      or   no slope change) */
	/*assert(
			(x1 - x0) * (x2 - x3) < EP
					&& ((x3 - x0) * (x1 - x2) < EP || xb * xb < xa * xc + EP));
	assert(
			(y1 - y0) * (y2 - y3) < EP
					&& ((y3 - y0) * (y1 - y2) < EP || yb * yb < ya * yc + EP));*/

	if (xa == 0 && ya == 0) { /* quadratic Bezier */
		sx = floor((3 * x1 - x0 + 1) / 2);
		sy = floor((3 * y1 - y0 + 1) / 2); /* new midpoint32_t */
		return drawBezierSegment(x0, y0, sx, sy, x3, y3, color);
	}
	x1 = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + 1; /* line lengths */
	x2 = (x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3) + 1;
	do { /* loop over both ends */
		ab = xa * yb - xb * ya;
		ac = xa * yc - xc * ya;
		bc = xb * yc - xc * yb;
		ex = ab * (ab + ac - 3 * bc) + ac * ac; /* P0 part of self-intersection loop? */
		f = ex > 0 ? 1 : sqrt(1 + 1024 / x1); /* calculate resolution */
		ab *= f;
		ac *= f;
		bc *= f;
		ex *= f * f; /* increase resolution */
		xy = 9 * (ab + ac + bc) / 8;
		cb = 8 * (xa - ya); /* init differences of 1st degree */
		dx = 27 * (8 * ab * (yb * yb - ya * yc) + ex * (ya + 2 * yb + yc)) / 64
				- ya * ya * (xy - ya);
		dy = 27 * (8 * ab * (xb * xb - xa * xc) - ex * (xa + 2 * xb + xc)) / 64
				- xa * xa * (xy + xa);
		/* init differences of 2nd degree */
		xx = 3 * (3 * ab * (3 * yb * yb - ya * ya - 2 * ya * yc)
						- ya * (3 * ac * (ya + yb) + ya * cb)) / 4;
		yy = 3 * (3 * ab * (3 * xb * xb - xa * xa - 2 * xa * xc)
						- xa * (3 * ac * (xa + xb) + xa * cb)) / 4;
		xy = xa * ya * (6 * ab + 6 * ac - 3 * bc + cb);
		ac = ya * ya;
		cb = xa * xa;
		xy = 3 * (xy + 9 * f * (cb * yb * yc - xb * xc * ac)
						- 18 * xb * yb * ab) / 8;

		if (ex < 0) { /* negate values if inside self-intersection loop */
			dx = -dx;
			dy = -dy;
			xx = -xx;
			yy = -yy;
			xy = -xy;
			ac = -ac;
			cb = -cb;
		} /* init differences of 3rd degree */
		ab = 6 * ya * ac;
		ac = -6 * xa * ac;
		bc = 6 * ya * cb;
		cb = -6 * xa * cb;
		dx += xy;
		ex = dx + dy;
		dy += xy; /* error of 1st step */

		display->startWrite();
		for (pxy = &xy, fx = fy = f; x0 != x3 && y0 != y3;) {
			display->writePixel(x0, y0, color); /* plot curve */
			do { /* move sub-steps of one pixel */
				if (dx > *pxy || dy < *pxy)
					goto exit;
				/* confusing values */
				y1 = 2 * ex - dy; /* save value for test of y step */
				if (2 * ex >= dx) { /* x sub-step */
					fx--;
					ex += dx += xx;
					dy += xy += ac;
					yy += bc;
					xx += ab;
				}
				if (y1 <= 0) { /* y sub-step */
					fy--;
					ex += dy += yy;
					dx += xy += bc;
					xx += ac;
					yy += cb;
				}
			} while (fx > 0 && fy > 0); /* pixel complete? */
			if (2 * fx <= f) {
				x0 += sx;
				fx += f;
			} /* x step */
			if (2 * fy <= f) {
				y0 += sy;
				fy += f;
			} /* y step */
			if (pxy == &xy && dx < 0 && dy > 0)
				pxy = &EP; /* pixel ahead valid */
			yield();
		}
		exit: xx = x0;
		x0 = x3;
		x3 = xx;
		sx = -sx;
		xb = -xb; /* swap legs */
		yy = y0;
		y0 = y3;
		y3 = yy;
		sy = -sy;
		yb = -yb;
		x1 = x2;
	} while (leg--); /* try other end */
	display->writeLine(x0, y0, x3, y3, color); /* remaining part in case of cusp or crunode */
	display->endWrite();
}

void Complex_GFX::drawCubicBezier(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3,
		int32_t y3, uint16_t color) { /* plot any cubic Bezier curve */
	int32_t n = 0, i = 0;
	long xc = x0 + x1 - x2 - x3, xa = xc - 4 * (x1 - x2);
	long xb = x0 - x1 - x2 + x3, xd = xb + 4 * (x1 + x2);
	long yc = y0 + y1 - y2 - y3, ya = yc - 4 * (y1 - y2);
	long yb = y0 - y1 - y2 + y3, yd = yb + 4 * (y1 + y2);
	FPoint fx0 = x0, fx1, fx2, fx3, fy0 = y0, fy1, fy2, fy3;
	FPoint t1 = xb * xb - xa * xc, t2, t[5];
	/* sub-divide curve at gradient sign changes */
	if (xa == 0) { /* horizontal */
		if (abs(xc) < 2 * abs(xb))
			t[n++] = xc / (2.0f * xb); /* one change */
	} else if (t1 > 0.0f) { /* two changes */
		t2 = sqrt(t1);
		t1 = (xb - t2) / xa;
		if (fabs(t1) < 1.0f)
			t[n++] = t1;
		t1 = (xb + t2) / xa;
		if (fabs(t1) < 1.0f)
			t[n++] = t1;
	}
	t1 = yb * yb - ya * yc;
	if (ya == 0) { /* vertical */
		if (abs(yc) < 2 * abs(yb))
			t[n++] = yc / (2.0f * yb); /* one change */
	} else if (t1 > 0.0f) { /* two changes */
		t2 = sqrt(t1);
		t1 = (yb - t2) / ya;
		if (fabs(t1) < 1.0f)
			t[n++] = t1;
		t1 = (yb + t2) / ya;
		if (fabs(t1) < 1.0f)
			t[n++] = t1;
	}
	for (i = 1; i < n; i++) /* bubble sort of 4 points */
		if ((t1 = t[i - 1]) > t[i]) {
			t[i - 1] = t[i];
			t[i] = t1;
			i = 0;
		}

	t1 = -1.0f;
	t[n] = 1.0f; /* begin / end point32_t */
	for (i = 0; i <= n; i++) { /* plot each segment separately */
		t2 = t[i]; /* sub-divide at t[i-1], t[i] */
		fx1 = (t1 * (t1 * xb - 2 * xc) - t2 * (t1 * (t1 * xa - 2 * xb) + xc)
				+ xd) / 8 - fx0;
		fy1 = (t1 * (t1 * yb - 2 * yc) - t2 * (t1 * (t1 * ya - 2 * yb) + yc)
				+ yd) / 8 - fy0;
		fx2 = (t2 * (t2 * xb - 2 * xc) - t1 * (t2 * (t2 * xa - 2 * xb) + xc)
				+ xd) / 8 - fx0;
		fy2 = (t2 * (t2 * yb - 2 * yc) - t1 * (t2 * (t2 * ya - 2 * yb) + yc)
				+ yd) / 8 - fy0;
		fx0 -= fx3 = (t2 * (t2 * (3 * xb - t2 * xa) - 3 * xc) + xd) / 8;
		fy0 -= fy3 = (t2 * (t2 * (3 * yb - t2 * ya) - 3 * yc) + yd) / 8;
		x3 = floor(fx3 + 0.5f);
		y3 = floor(fy3 + 0.5f); /* scale bounds to int32_t */
		if (fx0 != 0.0f) {
			fx1 *= fx0 = (x0 - x3) / fx0;
			fx2 *= fx0;
		}
		if (fy0 != 0.0f) {
			fy1 *= fy0 = (y0 - y3) / fy0;
			fy2 *= fy0;
		}
		if (x0 != x3 || y0 != y3) /* segment t1 - t2 */
			drawCubicBezierSeg(x0, y0, x0 + fx1, y0 + fy1, x0 + fx2, y0 + fy2,
					x3, y3, color);
		x0 = x3;
		y0 = y3;
		fx0 = fx3;
		fy0 = fy3;
		t1 = t2;
	}
}

void Complex_GFX::drawRationalBezierSeg(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2,
		int32_t y2, FPoint w, uint16_t color) { /* plot a limited rational Bezier segment, squared weight */
	int32_t sx = x2 - x1, sy = y2 - y1; /* relative values for checks */
	FPoint dx = x0 - x2, dy = y0 - y2, xx = x0 - x1, yy = y0 - y1;
	FPoint xy = xx * sy + yy * sx, cur = xx * sy - yy * sx, err; /* curvature */

	// assert(xx * sx <= 0.0f && yy * sy <= 0.0f); /* sign of gradient must not change */

	if (cur != 0.0f && w > 0.0f) { /* no straight line */
		if (sx * (long) sx + sy * (long) sy > xx * xx + yy * yy) { /* begin with longer part */
			x2 = x0;
			x0 -= dx;
			y2 = y0;
			y0 -= dy;
			cur = -cur; /* swap P0 P2 */
		}
		xx = 2.0f * (4.0f * w * sx * xx + dx * dx); /* differences 2nd degree */
		yy = 2.0f * (4.0f * w * sy * yy + dy * dy);
		sx = x0 < x2 ? 1 : -1; /* x step direction */
		sy = y0 < y2 ? 1 : -1; /* y step direction */
		xy = -2.0f * sx * sy * (2.0f * w * xy + dx * dy);

		if (cur * sx * sy < 0.0f) { /* negated curvature? */
			xx = -xx;
			yy = -yy;
			xy = -xy;
			cur = -cur;
		}
		dx = 4.0f * w * (x1 - x0) * sy * cur + xx / 2.0f + xy; /* differences 1st degree */
		dy = 4.0f * w * (y0 - y1) * sx * cur + yy / 2.0f + xy;

		if (w < 0.5f && (dy > xy || dx < xy)) { /* flat ellipse, algorithm fails */
			cur = (w + 1.0f) / 2.0f;
			w = sqrt(w);
			xy = 1.0f / (w + 1.0f);
			sx = floor((x0 + 2.0f * w * x1 + x2) * xy / 2.0f + 0.5f); /* subdivide curve in half */
			sy = floor((y0 + 2.0f * w * y1 + y2) * xy / 2.0f + 0.5f);
			dx = floor((w * x1 + x0) * xy + 0.5f);
			dy = floor((y1 * w + y0) * xy + 0.5f);
			drawRationalBezierSeg(x0, y0, dx, dy, sx, sy, cur, color);/* plot separately */
			dx = floor((w * x1 + x2) * xy + 0.5f);
			dy = floor((y1 * w + y2) * xy + 0.5f);
			drawRationalBezierSeg(sx, sy, dx, dy, x2, y2, cur, color);
			return;
		}
		err = dx + dy - xy; /* error 1.step */
		display->startWrite();
		do {
			display->writePixel(x0, y0, color); /* plot curve */
			if (x0 == x2 && y0 == y2)
				return; /* last pixel -> curve finished */
			x1 = 2 * err > dy;
			y1 = 2 * (err + yy) < -dy;/* save value for test of x step */
			if (2 * err < dx || y1) {
				y0 += sy;
				dy += xy;
				err += dx += xx;
			}/* y step */
			if (2 * err > dx || x1) {
				x0 += sx;
				dx += xy;
				err += dy += yy;
			}/* x step */
			yield();
		} while (dy <= xy && dx >= xy); /* gradient negates -> algorithm fails */
	}
	display->writeLine(x0, y0, x2, y2, color); /* plot remaining needle to end */
	display->endWrite();
}

void Complex_GFX::drawRationalBezier(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2,
		int32_t y2, FPoint w, uint16_t color) { /* plot any quadratic rational Bezier curve */
	int32_t x = x0 - 2 * x1 + x2, y = y0 - 2 * y1 + y2;
	FPoint xx = x0 - x1, yy = y0 - y1, ww, t, q;

	//assert(w >= 0.0f);

	if (xx * (x2 - x1) > 0) { /* horizontal cut at P4? */
		if (yy * (y2 - y1) > 0) /* vertical cut at P6 too? */
			if (fabs(xx * y) > fabs(yy * x)) { /* which first? */
				x0 = x2;
				x2 = xx + x1;
				y0 = y2;
				y2 = yy + y1; /* swap points */
			} /* now horizontal cut at P4 comes first */
		if (x0 == x2 || w == 1.0f)
			t = (x0 - x1) / (float) x;
		else { /* non-rational or rational case */
			q = sqrt(
					4.0f * w * w * (x0 - x1) * (x2 - x1)
							+ (x2 - x0) * (long) (x2 - x0));
			if (x1 < x0)
				q = -q;
			t = (2.0f * w * (x0 - x1) - x0 + x2 + q)
					/ (2.0f * (1.0f - w) * (x2 - x0)); /* t at P4 */
		}
		q = 1.0f / (2.0f * t * (1.0f - t) * (w - 1.0f) + 1.0f); /* sub-divide at t */
		xx = (t * t * (x0 - 2.0f * w * x1 + x2) + 2.0f * t * (w * x1 - x0) + x0)
				* q; /* = P4 */
		yy = (t * t * (y0 - 2.0f * w * y1 + y2) + 2.0f * t * (w * y1 - y0) + y0)
				* q;
		ww = t * (w - 1.0f) + 1.0f;
		ww *= ww * q; /* squared weight P3 */
		w = ((1.0f - t) * (w - 1.0f) + 1.0f) * sqrt(q); /* weight P8 */
		x = floor(xx + 0.5f);
		y = floor(yy + 0.5f); /* P4 */
		yy = (xx - x0) * (y1 - y0) / (x1 - x0) + y0; /* intersect P3 | P0 P1 */
		drawRationalBezierSeg(x0, y0, x, floor(yy + 0.5f), x, y, ww,
				color);
		yy = (xx - x2) * (y1 - y2) / (x1 - x2) + y2; /* intersect P4 | P1 P2 */
		y1 = floor(yy + 0.5f);
		x0 = x1 = x;
		y0 = y; /* P0 = P4, P1 = P8 */
	}
	if ((y0 - y1) * (long) (y2 - y1) > 0) { /* vertical cut at P6? */
		if (y0 == y2 || w == 1.0f)
			t = (y0 - y1) / (y0 - 2.0f * y1 + y2);
		else { /* non-rational or rational case */
			q = sqrt(
					4.0f * w * w * (y0 - y1) * (y2 - y1)
							+ (y2 - y0) * (long) (y2 - y0));
			if (y1 < y0)
				q = -q;
			t = (2.0f * w * (y0 - y1) - y0 + y2 + q)
					/ (2.0f * (1.0f - w) * (y2 - y0)); /* t at P6 */
		}
		q = 1.0f / (2.0f * t * (1.0f - t) * (w - 1.0f) + 1.0f); /* sub-divide at t */
		xx = (t * t * (x0 - 2.0f * w * x1 + x2) + 2.0f * t * (w * x1 - x0) + x0)
				* q; /* = P6 */
		yy = (t * t * (y0 - 2.0f * w * y1 + y2) + 2.0f * t * (w * y1 - y0) + y0)
				* q;
		ww = t * (w - 1.0f) + 1.0f;
		ww *= ww * q; /* squared weight P5 */
		w = ((1.0f - t) * (w - 1.0f) + 1.0f) * sqrt(q); /* weight P7 */
		x = floor(xx + 0.5f);
		y = floor(yy + 0.5f); /* P6 */
		xx = (x1 - x0) * (yy - y0) / (y1 - y0) + x0; /* intersect P6 | P0 P1 */
		drawRationalBezierSeg(x0, y0, floor(xx + 0.5f), y, x, y, ww, color);
		xx = (x1 - x2) * (yy - y2) / (y1 - y2) + x2; /* intersect P7 | P1 P2 */
		x1 = floor(xx + 0.5f);
		x0 = x;
		y0 = y1 = y; /* P0 = P6, P1 = P7 */
	}
	drawRationalBezierSeg(x0, y0, x1, y1, x2, y2, w * w, color); /* remaining */
}

void Complex_GFX::drawEllipseRect(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
		uint16_t color) { /* rectangular parameter enclosing the ellipse */
	long a = abs(x1 - x0), b = abs(y1 - y0), b1 = b & 1; /* diameter */
	FPoint dx = 4 * (1.0f - a) * b * b, dy = 4 * (b1 + 1) * a * a; /* error increment */
	FPoint err = dx + dy + b1 * a * a, e2; /* error of 1.step */

	if (x0 > x1) {
		x0 = x1;
		x1 += a;
	} /* if called with swapped points */
	if (y0 > y1)
		y0 = y1; /* .. exchange them */
	y0 += (b + 1) / 2;
	y1 = y0 - b1; /* starting pixel */
	a = 8 * a * a;
	b1 = 8 * b * b;

	display->startWrite();
	do {
		display->writePixel(x1, y0, color); /*   I. Quadrant */
		display->writePixel(x0, y0, color); /*  II. Quadrant */
		display->writePixel(x0, y1, color); /* III. Quadrant */
		display->writePixel(x1, y1, color); /*  IV. Quadrant */
		e2 = 2 * err;
		if (e2 <= dy) {
			y0++;
			y1--;
			err += dy += a;
		} /* y step */
		if (e2 >= dx || 2 * err > dy) {
			x0++;
			x1--;
			err += dx += b1;
		} /* x step */
		yield();
	} while (x0 <= x1);

	while (y0 - y1 <= b) { /* too early stop of flat ellipses a=1 */
		display->writePixel(x0 - 1, y0, color); /* -> finish tip of ellipse */
		display->writePixel(x1 + 1, y0++, color);
		display->writePixel(x0 - 1, y1, color);
		display->writePixel(x1 + 1, y1--, color);
		yield();
	}
	display->endWrite();
}

void Complex_GFX::drawRotatedEllipse(int32_t x, int32_t y, int32_t a, int32_t b, FPoint angle,
		uint16_t color) { /* plot ellipse rotated by angle (radian) */
	FPoint xd = (long) a * a, yd = (long) b * b;
	FPoint s = sin(angle), zd = (xd - yd) * s; /* ellipse rotation */
	xd = sqrt(static_cast<float>(xd - zd * s)),
	yd = sqrt(static_cast<float>(yd + zd * s)); /* surrounding rectangle */

	a = xd + 0.5f;
	b = yd + 0.5f;
	zd = zd * a * b / (xd * yd); /* scale to integer */
	drawRotatedEllipseRect(x - a, y - b, x + a, y + b,
			static_cast<long>(4 * zd * cos(angle)), color);
}

void Complex_GFX::drawRotatedEllipseRect(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
		long zd, uint16_t color) { /* rectangle enclosing the ellipse, integer rotation angle */
	int32_t xd = x1 - x0, yd = y1 - y0;
	FPoint w = xd * (long) yd;
	if (zd == 0)
		return drawEllipseRect(x0, y0, x1, y1, color); /* looks nicer */
	if (w != 0.0f)
		w = (w - zd) / (w + w); /* squared weight of P1 */
	//assert(w <= 1.0f && w >= 0.0f); /* limit angle to |zd|<=xd*yd */
	xd = floor(xd * w + 0.5f);
	yd = floor(yd * w + 0.5f); /* snap xe,ye to int32_t */
	drawRationalBezierSeg(x0, y0 + yd, x0, y0, x0 + xd, y0, 1.0f - w, color);
	drawRationalBezierSeg(x0, y0 + yd, x0, y1, x1 - xd, y1, w, color);
	drawRationalBezierSeg(x1, y1 - yd, x1, y1, x1 - xd, y1, 1.0f - w, color);
	drawRationalBezierSeg(x1, y1 - yd, x1, y0, x0 + xd, y0, w, color);
}

