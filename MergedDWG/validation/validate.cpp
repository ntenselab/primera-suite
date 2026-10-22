#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <random>
#include <cassert>
#define __DEVICE__
#define __TEXTURE__ const float*
struct float3 { float x,y,z; };
float3 make_float3(float x,float y,float z){return {x,y,z};}
float3 operator+(float3 a,float3 b){return {a.x+b.x,a.y+b.y,a.z+b.z};}
float3 operator-(float3 a,float3 b){return {a.x-b.x,a.y-b.y,a.z-b.z};}
float3 operator*(float3 a,float b){return {a.x*b,a.y*b,a.z*b};}
float3& operator*=(float3& a,float b){a=a*b;return a;}
float _mix(float a,float b,float t){return a+(b-a)*t;}
float3 _mix(float3 a,float3 b,float t){return a+(b-a)*t;}
float _clampf(float x,float a,float b){return std::min(std::max(x,a),b);}
#define _fabs fabsf
#define _powf powf
#define _fminf fminf
#define _fmaxf fmaxf
#define _log2f log2f
#define _log10f log10f
#define _expf expf
#define _exp2f exp2f
#define _tanhf tanhf
#define _floorf floorf
#define _cosf cosf
#define _sinf sinf
#define _sqrtf sqrtf
int width;
float _tex2D(const float* t,int x,int y){return t[y*width+x];}

namespace merged {
/*
Primera Merged DWG v1.0
Adapted 2026-10-22. Original algorithms by Geoff Smith.
Source commit: 5b24e593390095968159e9ca55802feb0b35b64c
Input/output: DaVinci Wide Gamut / Intermediate. Not an output transform.
Order: Primary > Hue > Split > Skin. UI sections are static labels, not tabs.
Skin texture samples re-evaluate upstream stages to preserve serial-node semantics.
CPU validation is not Resolve/GPU compilation certification.

MIT License

Copyright (c) 2026 Geoff Smith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
int bypass = 0;
int enable_K = 1;
int enable_S = 1;
int enable_H = 1;
int enable_P = 1;
float P_p_exposure = 0.0;
float P_p_bp = 0.0;
float P_p_temp = 0.0;
float P_p_tint = 0.0;
float P_p_contrast = 1.0;
float P_p_pivot_offset = 0.0;
float P_p_shadows = 0.0;
float P_p_highlights = 0.0;
float P_p_high_soft = 0.0;
float P_p_neg_sat = 1.0;
float P_p_pos_sat = 0.0;
int P_p_preserve_lum = 0;
int P_p_show_chart = 0;
float H_p_red_hue = 0.0;
float H_p_red_den = 0.0;
float H_p_yel_hue = 0.0;
float H_p_yel_den = 0.0;
float H_p_grn_hue = 0.0;
float H_p_grn_den = 0.0;
float H_p_cyn_hue = 0.0;
float H_p_cyn_den = 0.0;
float H_p_blu_hue = 0.0;
float H_p_blu_den = 0.0;
float H_p_mag_hue = 0.0;
float H_p_mag_den = 0.0;
int H_p_preserve_lum = 0;
int H_p_soft_squeeze = 1;
int H_p_hard_squeeze = 0;
float H_p_cinecolor = 0.0;
int H_p_protect_skin = 0;
float S_pivot = 0.0;
float S_transition = 0.2;
float S_shadow_red = 0.0;
float S_shadow_green = 0.0;
float S_shadow_blue = 0.0;
float S_shadow_cyan = 0.0;
float S_shadow_magenta = 0.0;
float S_shadow_yellow = 0.0;
float S_highlight_red = 0.0;
float S_highlight_green = 0.0;
float S_highlight_blue = 0.0;
float S_highlight_cyan = 0.0;
float S_highlight_magenta = 0.0;
float S_highlight_yellow = 0.0;
float S_preserve_luminance = 0.8;
int S_opt_showcurve = 0;
int S_visualize = 0;
int S_opt_showramp = 0;
float S_ramp_pos = 0.0;
int S_p_show_chart = 0;
float K_p_hue = 0.0;
float K_p_saturation = 0.0;
float K_p_density = 0.0;
float K_p_range = 1.0;
float K_p_soften = 0.0;
float K_p_evenness = 0.0;
float K_p_separation = 0.0;
float K_p_lo_gate = 0.0;
float K_p_hi_gate = 1.0;
int K_p_show_mask = 0;
int K_p_legend = 0;
typedef struct {
    float P_p_exposure;
    float P_p_bp;
    float P_p_temp;
    float P_p_tint;
    float P_p_contrast;
    float P_p_pivot_offset;
    float P_p_shadows;
    float P_p_highlights;
    float P_p_high_soft;
    float P_p_neg_sat;
    float P_p_pos_sat;
    int P_p_preserve_lum;
    int P_p_show_chart;
    float H_p_red_hue;
    float H_p_red_den;
    float H_p_yel_hue;
    float H_p_yel_den;
    float H_p_grn_hue;
    float H_p_grn_den;
    float H_p_cyn_hue;
    float H_p_cyn_den;
    float H_p_blu_hue;
    float H_p_blu_den;
    float H_p_mag_hue;
    float H_p_mag_den;
    int H_p_preserve_lum;
    int H_p_soft_squeeze;
    int H_p_hard_squeeze;
    float H_p_cinecolor;
    int H_p_protect_skin;
    float S_pivot;
    float S_transition;
    float S_shadow_red;
    float S_shadow_green;
    float S_shadow_blue;
    float S_shadow_cyan;
    float S_shadow_magenta;
    float S_shadow_yellow;
    float S_highlight_red;
    float S_highlight_green;
    float S_highlight_blue;
    float S_highlight_cyan;
    float S_highlight_magenta;
    float S_highlight_yellow;
    float S_preserve_luminance;
    int S_opt_showcurve;
    int S_visualize;
    int S_opt_showramp;
    float S_ramp_pos;
    int S_p_show_chart;
    float K_p_hue;
    float K_p_saturation;
    float K_p_density;
    float K_p_range;
    float K_p_soften;
    float K_p_evenness;
    float K_p_separation;
    float K_p_lo_gate;
    float K_p_hi_gate;
    int K_p_show_mask;
    int K_p_legend;
    int enable_P;
    int enable_H;
    int enable_S;
    int enable_K;
} PrimeraParams;
// Primera v0.6.0
// https://github.com/geoffsmithBK/primera-suite

// ============================================================
// UI Parameters
// ===============================================================

// ============================================================
// Luminance
// ============================================================

__DEVICE__ float P_luminance(float r, float g, float b) {
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

__DEVICE__ float P_luminance_dwg(float3 rgb) {
    return 0.2627f * rgb.x + 0.6780f * rgb.y + 0.0593f * rgb.z;
}

// ============================================================
// HSV conversion
// ============================================================

__DEVICE__ float3 P_rgb_to_hsv(float3 rgb) {
    float r = rgb.x, g = rgb.y, b = rgb.z;
    float cmax = _fmaxf(r, _fmaxf(g, b));
    float cmin = _fminf(r, _fminf(g, b));
    float delta = cmax - cmin;

    float h = 0.0f;
    if (delta > 0.0f) {
        if (cmax == r)      h = (g - b) / delta;
        else if (cmax == g) h = (b - r) / delta + 2.0f;
        else                h = (r - g) / delta + 4.0f;
        if (h < 0.0f) h += 6.0f;
        h /= 6.0f;
    }

    float s = (cmax > 0.0f) ? delta / cmax : 0.0f;
    return make_float3(h, s, cmax);
}

__DEVICE__ float3 P_hsv_to_rgb(float3 hsv) {
    float h = hsv.x * 6.0f;
    float s = hsv.y;
    float v = hsv.z;

    float c = v * s;
    float x = c * (1.0f - _fabs(h - 2.0f * _floorf(h * 0.5f) - 1.0f));
    float m = v - c;

    float r, g, b;
    if      (h < 1.0f) { r = c; g = x; b = 0.0f; }
    else if (h < 2.0f) { r = x; g = c; b = 0.0f; }
    else if (h < 3.0f) { r = 0.0f; g = c; b = x; }
    else if (h < 4.0f) { r = 0.0f; g = x; b = c; }
    else if (h < 5.0f) { r = x; g = 0.0f; b = c; }
    else               { r = c; g = 0.0f; b = x; }

    return make_float3(r + m, g + m, b + m);
}
// ============================================================
// Transfer functions: linear -> log
// ============================================================

__DEVICE__ float P_lin_to_logc3(float x) {
    const float a = 5.555556f;
    const float b = 0.052272f;
    const float c = 0.247190f;
    const float d = 0.385537f;
    const float e = 5.367655f;
    const float f = 0.092809f;
    const float cut = 0.010591f;
    return (x > cut) ? c * _log10f(a * x + b) + d : e * x + f;
}

__DEVICE__ float P_lin_to_logc4(float x) {
    const float a = (262144.0f - 16.0f) / 117.45f;
    const float b = (1023.0f - 95.0f) / 1023.0f;
    const float c = 95.0f / 1023.0f;
    const float s = (7.0f * 0.6931471805599453f * _powf(2.0f, 7.0f - 14.0f * c / b)) / (a * b);
    const float t = (_powf(2.0f, 14.0f * (-c / b) + 6.0f) - 64.0f) / a;
    return (x >= t) ? (_log2f(a * x + 64.0f) - 6.0f) / 14.0f * b + c : (x - t) / s;
}

__DEVICE__ float P_lin_to_redlog3g10(float x) {
    const float a = 0.224282f;
    const float b = 155.975327f;
    const float c = 0.01f;
    const float g = 15.1927f;
    float xc = x + c;
    return (xc < 0.0f) ? xc * g : a * _log10f(xc * b + 1.0f);
}

__DEVICE__ float P_lin_to_slog3(float x) {
    const float cut = 0.01125f;
    if (x >= cut)
        return (420.0f + _log10f((x + 0.01f) / 0.19f) * 261.5f) / 1023.0f;
    else
        return (x * (171.2102946929f - 95.0f) / 0.01125f + 95.0f) / 1023.0f;
}

__DEVICE__ float P_lin_to_acescct(float x) {
    const float cut = 0.0078125f;
    const float slope = 10.5402377416545f;
    const float offset = 0.0729055341958355f;
    return (x > cut) ? (_log2f(x) + 9.72f) / 17.52f : slope * x + offset;
}

__DEVICE__ float P_lin_to_di(float x) {
    const float A = 0.0075f;
    const float B = 7.0f;
    const float C = 0.07329248f;
    const float M = 10.44426855f;
    const float LIN_CUT = 0.00262409f;
    return (x <= LIN_CUT) ? x * M : C * (_log2f(x + A) + B);
}

__DEVICE__ float P_lin_to_cineon(float x) {
    const float offset = 0.0108f;
    return (_log10f(x * (1.0f - offset) + offset) * 300.0f + 685.0f) / 1023.0f;
}

__DEVICE__ float P_lin_to_flog2(float x) {
    const float a = 5.555556f;
    const float b = 0.064829f;
    const float c = 0.245281f;
    const float d = 0.384316f;
    const float e = 8.799461f;
    const float f = 0.092864f;
    const float cut1 = 0.000889f;
    return (x >= cut1) ? c * _log10f(a * x + b) + d : e * x + f;
}

__DEVICE__ float P_lin_to_vlog(float x) {
    const float b = 0.00873f;
    const float c = 0.241514f;
    const float d = 0.598206f;
    const float cut1 = 0.01f;
    return (x < cut1) ? 5.6f * x + 0.125f : c * _log10f(x + b) + d;
}

__DEVICE__ float P_encode(float x, int tf) {
    switch (tf) {
        case 0: return P_lin_to_logc3(x);
        case 1: return P_lin_to_logc4(x);
        case 2: return P_lin_to_redlog3g10(x);
        case 3: return P_lin_to_slog3(x);
        case 4: return P_lin_to_acescct(x);
        case 5: return P_lin_to_di(x);
        case 6: return P_lin_to_cineon(x);
        case 7: return P_lin_to_flog2(x);
        case 8: return P_lin_to_vlog(x);
        default: return x;
    }
}// ============================================================
// Transfer functions: log -> linear
// ============================================================

__DEVICE__ float P_logc3_to_lin(float x) {
    const float a = 5.555556f;
    const float b = 0.052272f;
    const float c = 0.247190f;
    const float d = 0.385537f;
    const float e = 5.367655f;
    const float f = 0.092809f;
    const float cut = 0.010591f;
    const float ecf = e * cut + f;
    return (x > ecf) ? (_powf(10.0f, (x - d) / c) - b) / a : (x - f) / e;
}

__DEVICE__ float P_logc4_to_lin(float x) {
    const float a = (262144.0f - 16.0f) / 117.45f;
    const float b = (1023.0f - 95.0f) / 1023.0f;
    const float c = 95.0f / 1023.0f;
    const float s = (7.0f * 0.6931471805599453f * _powf(2.0f, 7.0f - 14.0f * c / b)) / (a * b);
    const float t = (_powf(2.0f, 14.0f * (-c / b) + 6.0f) - 64.0f) / a;
    return (x >= 0.0f) ? (_powf(2.0f, 14.0f * ((x - c) / b) + 6.0f) - 64.0f) / a : x * s + t;
}

__DEVICE__ float P_redlog3g10_to_lin(float y) {
    const float a = 0.224282f;
    const float b = 155.975327f;
    const float c = 0.01f;
    const float g = 15.1927f;
    return (y < 0.0f) ? (y / g) - c : (_powf(10.0f, y / a) - 1.0f) / b - c;
}

__DEVICE__ float P_slog3_to_lin(float x) {
    const float cut = 171.2102946929f / 1023.0f;
    if (x >= cut)
        return _powf(10.0f, (x * 1023.0f - 420.0f) / 261.5f) * 0.19f - 0.01f;
    else
        return (x * 1023.0f - 95.0f) * 0.01125f / (171.2102946929f - 95.0f);
}

__DEVICE__ float P_acescct_to_lin(float x) {
    const float cut = 0.155251141552511f;
    const float slope = 10.5402377416545f;
    const float offset = 0.0729055341958355f;
    return (x > cut) ? _powf(2.0f, x * 17.52f - 9.72f) : (x - offset) / slope;
}

__DEVICE__ float P_di_to_lin(float x) {
    const float A = 0.0075f;
    const float B = 7.0f;
    const float C = 0.07329248f;
    const float M = 10.44426855f;
    const float LOG_CUT = 0.02740668f;
    return (x <= LOG_CUT) ? x / M : _powf(2.0f, x / C - B) - A;
}

__DEVICE__ float P_cineon_to_lin(float x) {
    const float offset = 0.0108f;
    return (_powf(10.0f, (x * 1023.0f - 685.0f) / 300.0f) - offset) / (1.0f - offset);
}

__DEVICE__ float P_flog2_to_lin(float x) {
    const float a = 5.555556f;
    const float b = 0.064829f;
    const float c = 0.245281f;
    const float d = 0.384316f;
    const float e = 8.799461f;
    const float f = 0.092864f;
    const float cut2 = 0.100686685370811f;
    return (x >= cut2) ? (_powf(10.0f, (x - d) / c) / a) - (b / a) : (x - f) / e;
}

__DEVICE__ float P_vlog_to_lin(float x) {
    const float b = 0.00873f;
    const float c = 0.241514f;
    const float d = 0.598206f;
    const float cut2 = 0.181f;
    return (x < cut2) ? (x - 0.125f) / 5.6f : _powf(10.0f, (x - d) / c) - b;
}

__DEVICE__ float P_decode(float x, int tf) {
    switch (tf) {
        case 0: return P_logc3_to_lin(x);
        case 1: return P_logc4_to_lin(x);
        case 2: return P_redlog3g10_to_lin(x);
        case 3: return P_slog3_to_lin(x);
        case 4: return P_acescct_to_lin(x);
        case 5: return P_di_to_lin(x);
        case 6: return P_cineon_to_lin(x);
        case 7: return P_flog2_to_lin(x);
        case 8: return P_vlog_to_lin(x);
        default: return x;
    }
}
// ============================================================
// 5x7 bitmap glyph table
// Each row returns 5 bits (LSB = rightmost column).
// Used by chart overlays and the PrimeraSkin Show Mask legend.
// ============================================================

__DEVICE__ int P_glyph_row(int ch, int row) {
    switch (ch) {
        case '0': return (row==0||row==6)?31:27;
        case '1': return row==1?14:row==6?31:6;
        case '2': return (row==0||row==3||row==6)?31:row<3?3:24;
        case '3': return (row==0||row==3||row==6)?31:3;
        case '4': return row<=2?27:row==3?31:3;
        case '5': return (row==0||row==3||row==6)?31:row<3?24:3;
        case '6': return (row==0||row==3||row==6)?31:row<3?24:27;
        case '7': return row==0?31:row<=2?3:6;
        case '8': return (row==0||row==3||row==6)?31:27;
        case '9': return (row==0||row==3||row==6)?31:row<3?27:3;
        case 'A': return (row==0||row==3)?31:27;
        case 'C': return (row==0||row==6)?31:24;
        case 'D': return (row==0||row==6)?30:27;
        case 'E': return (row==0||row==3||row==6)?31:24;
        case 'F': return (row==0||row==3)?31:24;
        case 'G': return (row==0||row==6)?31:row<=2?24:27;
        case 'I': return (row==0||row==6)?31:6;
        case 'K': return row==0?17:row==1?18:row==2?20:row==3?24:row==4?20:row==5?18:17;
        case 'L': return row==6?31:24;
        case 'M': return row==0?17:row<=2?27:row==3?21:17;
        case 'N': return (row==0||row==6)?17:row<=2?25:row==3?21:19;
        case 'O': return (row==0||row==6)?31:27;
        case 'R': return (row==0||row==3)?31:row<=2?27:row==4?26:25;
        case 'S': return (row==0||row==3||row==6)?31:row<3?24:3;
        case 'T': return row==0?31:6;
        case 'Y': return row<=1?27:row==2?14:6;
        case '%': return row==0?25:row==1?26:row==2?2:row==3?4:row==4?8:row==5?19:3;
        case '=': return (row==2||row==4)?31:0;
        case '.': return row>=5?12:0;
        default:  return 0;
    }
}

// ============================================================
// Stop chart (renderer)
// Requires: P_encode(), P_glyph_row()
// ============================================================

__DEVICE__ int P_tf_name_char(int tf, int pos) {
    switch (tf) {
        case 0:
            if (pos==0) return 'L'; if (pos==1) return 'O'; if (pos==2) return 'G';
            if (pos==3) return 'C'; if (pos==4) return '3'; return 0;
        case 1:
            if (pos==0) return 'L'; if (pos==1) return 'O'; if (pos==2) return 'G';
            if (pos==3) return 'C'; if (pos==4) return '4'; return 0;
        case 2:
            if (pos==0) return 'R'; if (pos==1) return 'E'; if (pos==2) return 'D';
            if (pos==3) return 'L'; if (pos==4) return 'O'; if (pos==5) return 'G';
            if (pos==6) return '3'; if (pos==7) return 'G'; if (pos==8) return '1';
            if (pos==9) return '0'; return 0;
        case 3:
            if (pos==0) return 'S'; if (pos==1) return 'L'; if (pos==2) return 'O';
            if (pos==3) return 'G'; if (pos==4) return '3'; return 0;
        case 4:
            if (pos==0) return 'A'; if (pos==1) return 'C'; if (pos==2) return 'E';
            if (pos==3) return 'S'; if (pos==4) return 'C'; if (pos==5) return 'C';
            if (pos==6) return 'T'; return 0;
        case 5:
            if (pos==0) return 'D'; if (pos==1) return 'I'; return 0;
        case 6:
            if (pos==0) return 'C'; if (pos==1) return 'I'; if (pos==2) return 'N';
            if (pos==3) return 'E'; if (pos==4) return 'O'; if (pos==5) return 'N';
            return 0;
        case 7:
            if (pos==0) return 'F'; if (pos==1) return 'L'; if (pos==2) return 'O';
            if (pos==3) return 'G'; if (pos==4) return '2'; return 0;
        default: return 0;
    }
}

__DEVICE__ int P_tf_name_len(int tf) {
    switch (tf) {
        case 0: return 5;  case 1: return 5;  case 2: return 10;
        case 3: return 5;  case 4: return 7;  case 5: return 2;
        case 6: return 6;  case 7: return 5;  default: return 0;
    }
}

__DEVICE__ float3 P_render_chart(int p_Width, int p_Height, int p_X, int p_Y, int tf) {
    int px = p_X;
    int py = p_Y;
    float x_norm = (float)px / (float)p_Width;
    float y_norm = (float)py / (float)p_Height;

    // Upper half: 17 stop columns (-8 to +8)
    if (y_norm < 0.5f) {
        int num_cols = 17;
        int col = (int)(x_norm * (float)num_cols);
        if (col >= num_cols) col = num_cols - 1;
        int stop = col - 8;
        float lin = 0.18f * _powf(2.0f, (float)stop);
        float val = P_encode(lin, tf);
        val = _fmaxf(0.0f, _fminf(val, 1.0f));
        return make_float3(val, val, val);
    }

    // Lower half: gradient + text overlay
    float val = x_norm;
    float mid_grey = P_encode(0.18f, tf);

    int cell = p_Height / 100;
    if (cell < 2) cell = 2;

    int mg_d1 = (int)(mid_grey * 10.0f) % 10;
    int mg_d2 = (int)(mid_grey * 100.0f) % 10;
    int mg_d3 = (int)(mid_grey * 1000.0f + 0.5f) % 10;

    int char_w = 6 * cell;
    int char_h = 7 * cell;
    int line_gap = 3 * cell;
    int pad = 2 * cell;

    int name_len = P_tf_name_len(tf);
    int line2_len = 16;

    int lower_top = p_Height / 2;
    int lower_h = p_Height - lower_top;
    int total_text_h = char_h * 2 + line_gap;
    int line1_top = lower_top + (lower_h - total_text_h) / 2;
    int line2_top = line1_top + char_h + line_gap;

    int line1_left = (p_Width - name_len * char_w + cell) / 2;
    int line2_left = (p_Width - line2_len * char_w + cell) / 2;

    int box_l = (line1_left < line2_left ? line1_left : line2_left) - pad;
    int box_r = line1_left + name_len * char_w;
    int box_r2 = line2_left + line2_len * char_w;
    if (box_r2 > box_r) box_r = box_r2;
    box_r += pad;
    int box_t = line1_top - pad;
    int box_b = line2_top + char_h + pad;

    int in_box = (px >= box_l && px < box_r && py >= box_t && py < box_b) ? 1 : 0;
    int text_hit = 0;

    if (py >= line1_top && py < line1_top + char_h) {
        int lx = px - line1_left;
        if (lx >= 0 && lx < name_len * char_w) {
            int ci = lx / char_w;
            int cx = (lx % char_w) / cell;
            int cy = (py - line1_top) / cell;
            if (cx < 5) {
                int ch = P_tf_name_char(tf, ci);
                if (ch > 0) {
                    int mask = P_glyph_row(ch, cy);
                    if ((mask >> (4 - cx)) & 1) text_hit = 1;
                }
            }
        }
    }

    if (text_hit == 0 && py >= line2_top && py < line2_top + char_h) {
        int lx = px - line2_left;
        if (lx >= 0 && lx < line2_len * char_w) {
            int ci = lx / char_w;
            int cx = (lx % char_w) / cell;
            int cy = (py - line2_top) / cell;
            if (cx < 5) {
                int ch = 0;
                switch (ci) {
                    case 0: ch='1'; break; case 1: ch='8'; break;
                    case 2: ch='%'; break; case 3: ch=' '; break;
                    case 4: ch='G'; break; case 5: ch='R'; break;
                    case 6: ch='A'; break; case 7: ch='Y'; break;
                    case 8: ch=' '; break; case 9: ch='='; break;
                    case 10: ch=' '; break; case 11: ch='0'; break;
                    case 12: ch='.'; break;
                    case 13: ch='0'+mg_d1; break;
                    case 14: ch='0'+mg_d2; break;
                    case 15: ch='0'+mg_d3; break;
                }
                if (ch > 32) {
                    int mask = P_glyph_row(ch, cy);
                    if ((mask >> (4 - cx)) & 1) text_hit = 1;
                }
            }
        }
    }

    if (text_hit == 1) {
        val = mid_grey + 0.12f;
    } else if (in_box == 1) {
        val = mid_grey;
    }

    return make_float3(val, val, val);
}

// ============================================================
// Tone mapping primitives
// ============================================================

__DEVICE__ float P_soft_black_point(float x, float bp, float knee) {
    if (x <= 0.0f) return x;
    float corrected = x - bp;
    if (corrected >= knee) return corrected;
    return knee * _expf(corrected / knee - 1.0f);
}

__DEVICE__ float P_shadow_fill(float x, float gain, float mid) {
    if (x <= 0.0f) return x;
    if (x >= mid) return x;
    float t = x / mid;
    float blend = t * t * (3.0f - 2.0f * t);
    return x * (gain + (1.0f - gain) * blend);
}

__DEVICE__ float P_highlight_gain(float x, float gain, float mid) {
    if (x <= mid) return x;
    if (x >= 1.0f) return x;
    float t = (x - mid) / (1.0f - mid);
    float blend = t * t * (3.0f - 2.0f * t);
    return x * (1.0f + (gain - 1.0f) * blend);
}

__DEVICE__ float P_soft_highlight(float x, float amount, float mid) {
    if (x <= 0.0f) return x;
    float knee_amount = _fminf(amount, 1.0f);
    float strength = 1.0f + _fmaxf(amount - 1.0f, 0.0f) * 2.0f;
    float knee = 1.0f - knee_amount * (1.0f - mid);
    if (x <= knee) return x;
    float range = 1.0f - knee;
    float t = (x - knee) / range * strength;
    return knee + range * _tanhf(t) / strength;
}

__DEVICE__ float P_rolling_contrast(float x, float contrast, float pivot) {
    if (x <= 0.0f) return x;
    if (x >= 1.0f) return x;
    if (x <= pivot) {
        return _powf(x / pivot, contrast) * pivot;
    } else {
        float inv = 1.0f - x;
        float inv_pivot = 1.0f - pivot;
        return 1.0f - _powf(inv / inv_pivot, contrast) * inv_pivot;
    }
}

// ============================================================
// Main Transform
// ============================================================

__DEVICE__ float3 P_transform(PrimeraParams u, int p_Width, int p_Height, int p_X, int p_Y, float p_R, float p_G, float p_B) {

    if (u.P_p_show_chart == 1) {
        float3 chart = P_render_chart(p_Width, p_Height, p_X, p_Y, 5);
        return chart;
    }

    float r = P_decode(p_R, 5);
    float g = P_decode(p_G, 5);
    float b = P_decode(p_B, 5);

    float gain = _powf(2.0f, u.P_p_exposure);
    r *= gain;
    g *= gain;
    b *= gain;

    if (u.P_p_bp != 0.0f) {
        float knee = 0.005f;
        float bp = -u.P_p_bp;
        r = P_soft_black_point(r, bp, knee);
        g = P_soft_black_point(g, bp, knee);
        b = P_soft_black_point(b, bp, knee);
    }

    r *= _powf(2.0f,  u.P_p_temp);
    b *= _powf(2.0f, -u.P_p_temp);
    g *= _powf(2.0f,  u.P_p_tint);

    r = P_encode(r, 5);
    g = P_encode(g, 5);
    b = P_encode(b, 5);

    if (u.P_p_contrast != 1.0f || u.P_p_pivot_offset != 0.0f) {
        float pivot = P_encode(0.18f, 5) + u.P_p_pivot_offset;
        pivot = _fmaxf(0.01f, _fminf(pivot, 0.99f));
        float con = (u.P_p_contrast != 1.0f) ? u.P_p_contrast : 1.0f;
        r = P_rolling_contrast(r, con, pivot);
        g = P_rolling_contrast(g, con, pivot);
        b = P_rolling_contrast(b, con, pivot);
    }

    if (u.P_p_shadows != 0.0f) {
        float mid = P_encode(0.18f, 5);
        float shd_gain = _powf(2.0f, u.P_p_shadows);
        r = P_shadow_fill(r, shd_gain, mid);
        g = P_shadow_fill(g, shd_gain, mid);
        b = P_shadow_fill(b, shd_gain, mid);
    }

    if (u.P_p_highlights != 0.0f) {
        float mid = P_encode(0.18f, 5);
        float hi_gain = _powf(2.0f, u.P_p_highlights);
        r = P_highlight_gain(r, hi_gain, mid);
        g = P_highlight_gain(g, hi_gain, mid);
        b = P_highlight_gain(b, hi_gain, mid);
    }

    if (u.P_p_high_soft > 0.0f) {
        float mid = P_encode(0.18f, 5);
        r = P_soft_highlight(r, u.P_p_high_soft, mid);
        g = P_soft_highlight(g, u.P_p_high_soft, mid);
        b = P_soft_highlight(b, u.P_p_high_soft, mid);
    }

    if (u.P_p_neg_sat < 1.0f && r >= 0.0f && g >= 0.0f && b >= 0.0f) {
        float luma_before = P_luminance(r, g, b);
        float luma = luma_before;
        r = luma + (r - luma) * u.P_p_neg_sat;
        g = luma + (g - luma) * u.P_p_neg_sat;
        b = luma + (b - luma) * u.P_p_neg_sat;

        if (u.P_p_preserve_lum == 1) {
            float luma_after = P_luminance(r, g, b);
            if (luma_after > 0.0f && luma_before > 0.0f) {
                float ratio = luma_before / luma_after;
                r *= ratio;
                g *= ratio;
                b *= ratio;
            }
        }
    }

    if (u.P_p_pos_sat > 0.0f && r >= 0.0f && g >= 0.0f && b >= 0.0f) {
        float luma_before = P_luminance(r, g, b);
        float3 hsv = P_rgb_to_hsv(make_float3(r, g, b));
        hsv.y *= _powf(2.0f, u.P_p_pos_sat);
        hsv.y = _fminf(hsv.y, 1.0f);
        float3 rgb_sat = P_hsv_to_rgb(hsv);
        r = rgb_sat.x;
        g = rgb_sat.y;
        b = rgb_sat.z;

        if (u.P_p_preserve_lum == 1) {
            float luma_after = P_luminance(r, g, b);
            if (luma_after > 0.0f && luma_before > 0.0f) {
                float ratio = luma_before / luma_after;
                r *= ratio;
                g *= ratio;
                b *= ratio;
            }
        }
    }

    return make_float3(r, g, b);
}

// PrimeraHue v0.6.0
// https://github.com/geoffsmithBK/primera-suite

// ============================================================
// UI Parameters
// ============================================================


// ============================================================
// Luminance
// ============================================================

__DEVICE__ float H_luminance(float r, float g, float b) {
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

__DEVICE__ float H_luminance_dwg(float3 rgb) {
    return 0.2627f * rgb.x + 0.6780f * rgb.y + 0.0593f * rgb.z;
}

// ============================================================
// HSV conversion
// ============================================================

__DEVICE__ float3 H_rgb_to_hsv(float3 rgb) {
    float r = rgb.x, g = rgb.y, b = rgb.z;
    float cmax = _fmaxf(r, _fmaxf(g, b));
    float cmin = _fminf(r, _fminf(g, b));
    float delta = cmax - cmin;

    float h = 0.0f;
    if (delta > 0.0f) {
        if (cmax == r)      h = (g - b) / delta;
        else if (cmax == g) h = (b - r) / delta + 2.0f;
        else                h = (r - g) / delta + 4.0f;
        if (h < 0.0f) h += 6.0f;
        h /= 6.0f;
    }

    float s = (cmax > 0.0f) ? delta / cmax : 0.0f;
    return make_float3(h, s, cmax);
}

__DEVICE__ float3 H_hsv_to_rgb(float3 hsv) {
    float h = hsv.x * 6.0f;
    float s = hsv.y;
    float v = hsv.z;

    float c = v * s;
    float x = c * (1.0f - _fabs(h - 2.0f * _floorf(h * 0.5f) - 1.0f));
    float m = v - c;

    float r, g, b;
    if      (h < 1.0f) { r = c; g = x; b = 0.0f; }
    else if (h < 2.0f) { r = x; g = c; b = 0.0f; }
    else if (h < 3.0f) { r = 0.0f; g = c; b = x; }
    else if (h < 4.0f) { r = 0.0f; g = x; b = c; }
    else if (h < 5.0f) { r = x; g = 0.0f; b = c; }
    else               { r = c; g = 0.0f; b = x; }

    return make_float3(r + m, g + m, b + m);
}

// ============================================================
// Skintone protection
// Requires: H_rgb_to_hsv()
// ============================================================

#define H_SKIN_HUE_CENTER (28.0f / 360.0f)
#define H_SKIN_HUE_WIDTH  (28.0f / 360.0f)

__DEVICE__ float H_hue_distance(float h1, float h2) {
    float d = _fabs(h1 - h2);
    return _fminf(d, 1.0f - d);
}

__DEVICE__ float H_smoothstep(float edge0, float edge1, float x) {
    float t = _clampf((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

__DEVICE__ float H_skin_mask_range(float r, float g, float b, float range) {
    float3 hsv = H_rgb_to_hsv(make_float3(r, g, b));
    float h = hsv.x;
    float s = hsv.y;

    if (s < 1e-6f || hsv.z < 1e-6f) return 0.0f;

    float width = H_SKIN_HUE_WIDTH * range;
    float dist = H_hue_distance(h, H_SKIN_HUE_CENTER);
    float hue_gate = 1.0f - H_smoothstep(0.0f, width, dist);
    float sat_gate = H_smoothstep(0.08f, 0.35f, s);

    return hue_gate * sat_gate;
}

__DEVICE__ float H_skin_mask(float r, float g, float b) {
    return H_skin_mask_range(r, g, b, 1.0f);
}

// Detection uses rg-chromaticity (normalized RGB ratios) rather than HSV
// saturation. Ratios are invariant to overall brightness, so the mask catches
// fair and dark skin equally well. Log-space skin clusters tightly around
// gn ≈ 0.33 with rn > bn.
__DEVICE__ float H_skin_mask_gated(float r, float g, float b, float range, float lo, float hi) {
    float sum = r + g + b;
    if (sum < 0.01f) return 0.0f;

    float rn = r / sum;
    float gn = g / sum;
    float bn = b / sum;

    // Skin chromaticity: gn sits in a narrow band, rn is red-leading vs bn.
    // Ramp widths are deliberately wide to match the smoothness of the
    // previous HSV-sat approach — narrow ramps make Density slider blotchy.
    float gn_gate = H_smoothstep(0.22f, 0.33f, gn) * (1.0f - H_smoothstep(0.37f, 0.48f, gn));
    float rb_gate = H_smoothstep(0.0f, 0.04f, rn - bn);
    float chroma_gate = gn_gate * rb_gate;

    // Hue gate preserves user-facing Range control (narrows/widens the pie slice)
    float3 hsv = H_rgb_to_hsv(make_float3(r, g, b));
    float h = hsv.x;
    float v = hsv.z;

    float width = H_SKIN_HUE_WIDTH * range;
    float dist = H_hue_distance(h, H_SKIN_HUE_CENTER);
    float hue_gate = 1.0f - H_smoothstep(0.0f, width, dist);

    // Value gates (user-facing Low/High Gate sliders)
    float lo_gate = (lo > 0.001f) ? H_smoothstep(lo * 0.5f, lo, v) : 1.0f;
    float hi_gate = (hi < 0.999f) ? 1.0f - H_smoothstep(hi - (1.0f - hi) * 0.5f, hi, v) : 1.0f;

    return hue_gate * chroma_gate * lo_gate * hi_gate;
}

// ============================================================
// Tetrahedral interpolation
// ============================================================

__DEVICE__ float3 H_tetra_corner(float3 identity, float hue, float density) {
    float angle = hue * 1.047198f;
    float mean = (identity.x + identity.y + identity.z) / 3.0f;
    float cx = identity.x - mean;
    float cy = identity.y - mean;
    float cz = identity.z - mean;

    float cos_a = _cosf(angle);
    float sin_a = _sinf(angle);
    float k = 0.57735026919f;

    float rx = cx * cos_a + k * (cz - cy) * sin_a;
    float ry = cy * cos_a + k * (cx - cz) * sin_a;
    float rz = cz * cos_a + k * (cy - cx) * sin_a;

    return make_float3(rx + mean + density, ry + mean + density, rz + mean + density);
}

__DEVICE__ float3 H_tetra_interp(float r, float g, float b,
                                float3 rc, float3 gc, float3 bc,
                                float3 cc, float3 mc, float3 yc) {
    float3 out;

    if (r >= g && r >= b) {
        if (g >= b) {
            out.x = r * rc.x + g * (yc.x - rc.x) + b * (1.0f - yc.x);
            out.y = r * rc.y + g * (yc.y - rc.y) + b * (1.0f - yc.y);
            out.z = r * rc.z + g * (yc.z - rc.z) + b * (1.0f - yc.z);
        } else {
            out.x = r * rc.x + b * (mc.x - rc.x) + g * (1.0f - mc.x);
            out.y = r * rc.y + b * (mc.y - rc.y) + g * (1.0f - mc.y);
            out.z = r * rc.z + b * (mc.z - rc.z) + g * (1.0f - mc.z);
        }
    } else if (g >= r && g >= b) {
        if (r >= b) {
            out.x = g * gc.x + r * (yc.x - gc.x) + b * (1.0f - yc.x);
            out.y = g * gc.y + r * (yc.y - gc.y) + b * (1.0f - yc.y);
            out.z = g * gc.z + r * (yc.z - gc.z) + b * (1.0f - yc.z);
        } else {
            out.x = g * gc.x + b * (cc.x - gc.x) + r * (1.0f - cc.x);
            out.y = g * gc.y + b * (cc.y - gc.y) + r * (1.0f - cc.y);
            out.z = g * gc.z + b * (cc.z - gc.z) + r * (1.0f - cc.z);
        }
    } else {
        if (r >= g) {
            out.x = b * bc.x + r * (mc.x - bc.x) + g * (1.0f - mc.x);
            out.y = b * bc.y + r * (mc.y - bc.y) + g * (1.0f - mc.y);
            out.z = b * bc.z + r * (mc.z - bc.z) + g * (1.0f - mc.z);
        } else {
            out.x = b * bc.x + g * (cc.x - bc.x) + r * (1.0f - cc.x);
            out.y = b * bc.y + g * (cc.y - bc.y) + r * (1.0f - cc.y);
            out.z = b * bc.z + g * (cc.z - bc.z) + r * (1.0f - cc.z);
        }
    }

    return out;
}

// ============================================================
// Soft gamut squeeze (tanh shoulder + exponential toe)
// ============================================================

__DEVICE__ float3 H_soft_squeeze(float3 rgb) {
    float r = rgb.x, g = rgb.y, b = rgb.z;

    const float knee = 0.9f;
    const float range = 0.1f;
    if (r > knee) { float t = (r - knee) / range; r = knee + range * _tanhf(t); }
    if (g > knee) { float t = (g - knee) / range; g = knee + range * _tanhf(t); }
    if (b > knee) { float t = (b - knee) / range; b = knee + range * _tanhf(t); }

    const float toe = 0.1f;
    if (r < toe) r = toe * _expf(r / toe - 1.0f);
    if (g < toe) g = toe * _expf(g / toe - 1.0f);
    if (b < toe) b = toe * _expf(b / toe - 1.0f);

    return make_float3(r, g, b);
}

// ============================================================
// Main Transform
// ============================================================

__DEVICE__ float3 H_transform(PrimeraParams u, int p_Width, int p_Height, int p_X, int p_Y, float p_R, float p_G, float p_B) {

    float r = p_R;
    float g = p_G;
    float b = p_B;

    if (r < 0.0f || r > 1.0f || g < 0.0f || g > 1.0f || b < 0.0f || b > 1.0f)
        return make_float3(r, g, b);

    int hue_den_active = (u.H_p_red_hue != 0.0f || u.H_p_red_den != 0.0f || u.H_p_grn_hue != 0.0f || u.H_p_grn_den != 0.0f ||
                          u.H_p_blu_hue != 0.0f || u.H_p_blu_den != 0.0f || u.H_p_cyn_hue != 0.0f || u.H_p_cyn_den != 0.0f ||
                          u.H_p_mag_hue != 0.0f || u.H_p_mag_den != 0.0f || u.H_p_yel_hue != 0.0f || u.H_p_yel_den != 0.0f) ? 1 : 0;
    int cinecolor_active = (u.H_p_cinecolor > 0.0f) ? 1 : 0;

    if (!hue_den_active && !cinecolor_active)
        return make_float3(r, g, b);

    float luma_before = H_luminance(r, g, b);

    if (hue_den_active) {
        float3 rc = H_tetra_corner(make_float3(1.0f, 0.0f, 0.0f), u.H_p_red_hue, u.H_p_red_den);
        float3 gc = H_tetra_corner(make_float3(0.0f, 1.0f, 0.0f), u.H_p_grn_hue, u.H_p_grn_den);
        float3 bc = H_tetra_corner(make_float3(0.0f, 0.0f, 1.0f), u.H_p_blu_hue, u.H_p_blu_den);
        float3 cc = H_tetra_corner(make_float3(0.0f, 1.0f, 1.0f), u.H_p_cyn_hue, u.H_p_cyn_den);
        float3 mc = H_tetra_corner(make_float3(1.0f, 0.0f, 1.0f), u.H_p_mag_hue, u.H_p_mag_den);
        float3 yc = H_tetra_corner(make_float3(1.0f, 1.0f, 0.0f), u.H_p_yel_hue, u.H_p_yel_den);

        if (u.H_p_hard_squeeze == 1) {
            rc.x = _fmaxf(0.0f, _fminf(rc.x, 1.0f)); rc.y = _fmaxf(0.0f, _fminf(rc.y, 1.0f)); rc.z = _fmaxf(0.0f, _fminf(rc.z, 1.0f));
            gc.x = _fmaxf(0.0f, _fminf(gc.x, 1.0f)); gc.y = _fmaxf(0.0f, _fminf(gc.y, 1.0f)); gc.z = _fmaxf(0.0f, _fminf(gc.z, 1.0f));
            bc.x = _fmaxf(0.0f, _fminf(bc.x, 1.0f)); bc.y = _fmaxf(0.0f, _fminf(bc.y, 1.0f)); bc.z = _fmaxf(0.0f, _fminf(bc.z, 1.0f));
            cc.x = _fmaxf(0.0f, _fminf(cc.x, 1.0f)); cc.y = _fmaxf(0.0f, _fminf(cc.y, 1.0f)); cc.z = _fmaxf(0.0f, _fminf(cc.z, 1.0f));
            mc.x = _fmaxf(0.0f, _fminf(mc.x, 1.0f)); mc.y = _fmaxf(0.0f, _fminf(mc.y, 1.0f)); mc.z = _fmaxf(0.0f, _fminf(mc.z, 1.0f));
            yc.x = _fmaxf(0.0f, _fminf(yc.x, 1.0f)); yc.y = _fmaxf(0.0f, _fminf(yc.y, 1.0f)); yc.z = _fmaxf(0.0f, _fminf(yc.z, 1.0f));
        }

        float3 result = H_tetra_interp(r, g, b, rc, gc, bc, cc, mc, yc);
        r = result.x;
        g = result.y;
        b = result.z;
    }

    if (u.H_p_preserve_lum == 1) {
        float luma_after = H_luminance(r, g, b);
        if (luma_after > 0.0f && luma_before > 0.0f) {
            float ratio = luma_before / luma_after;
            r *= ratio;
            g *= ratio;
            b *= ratio;
        }
    }

    if (cinecolor_active) {
        float cc_t = u.H_p_cinecolor;
        float3 cc_rc = make_float3(1.0f, 0.0f, 0.0f);
        float3 cc_gc = make_float3(0.0f, 1.0f, cc_t);
        float3 cc_bc = make_float3(0.0f, 0.0f, 1.0f - cc_t);
        float3 cc_cc = make_float3(0.0f, 1.0f, 1.0f);
        float3 cc_mc = make_float3(1.0f, 0.0f, 1.0f - cc_t);
        float3 cc_yc = make_float3(1.0f, 1.0f, cc_t);

        float cr = _fmaxf(0.0f, _fminf(r, 1.0f));
        float cg = _fmaxf(0.0f, _fminf(g, 1.0f));
        float cb = _fmaxf(0.0f, _fminf(b, 1.0f));

        float3 cc_result = H_tetra_interp(cr, cg, cb, cc_rc, cc_gc, cc_bc, cc_cc, cc_mc, cc_yc);

        if (u.H_p_protect_skin == 1) {
            float prot = H_skin_mask(cr, cg, cb);
            cc_result.z = cc_result.z * (1.0f - prot) + cb * prot;
        }

        r = cc_result.x;
        g = cc_result.y;
        b = cc_result.z;
    }

    if (u.H_p_soft_squeeze == 1) {
        float3 squeezed = H_soft_squeeze(make_float3(r, g, b));
        r = squeezed.x;
        g = squeezed.y;
        b = squeezed.z;
    }

    return make_float3(r, g, b);
}

// PrimeraSplit v0.6.0
// https://github.com/geoffsmithBK/primera-suite

// ============================================================
// UI Parameters
// ===============================================================




// ============================================================
// Luminance
// ============================================================

__DEVICE__ float S_luminance(float r, float g, float b) {
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

__DEVICE__ float S_luminance_dwg(float3 rgb) {
    return 0.2627f * rgb.x + 0.6780f * rgb.y + 0.0593f * rgb.z;
}
// ============================================================
// Transfer functions: linear -> log
// ============================================================

__DEVICE__ float S_lin_to_logc3(float x) {
    const float a = 5.555556f;
    const float b = 0.052272f;
    const float c = 0.247190f;
    const float d = 0.385537f;
    const float e = 5.367655f;
    const float f = 0.092809f;
    const float cut = 0.010591f;
    return (x > cut) ? c * _log10f(a * x + b) + d : e * x + f;
}

__DEVICE__ float S_lin_to_logc4(float x) {
    const float a = (262144.0f - 16.0f) / 117.45f;
    const float b = (1023.0f - 95.0f) / 1023.0f;
    const float c = 95.0f / 1023.0f;
    const float s = (7.0f * 0.6931471805599453f * _powf(2.0f, 7.0f - 14.0f * c / b)) / (a * b);
    const float t = (_powf(2.0f, 14.0f * (-c / b) + 6.0f) - 64.0f) / a;
    return (x >= t) ? (_log2f(a * x + 64.0f) - 6.0f) / 14.0f * b + c : (x - t) / s;
}

__DEVICE__ float S_lin_to_redlog3g10(float x) {
    const float a = 0.224282f;
    const float b = 155.975327f;
    const float c = 0.01f;
    const float g = 15.1927f;
    float xc = x + c;
    return (xc < 0.0f) ? xc * g : a * _log10f(xc * b + 1.0f);
}

__DEVICE__ float S_lin_to_slog3(float x) {
    const float cut = 0.01125f;
    if (x >= cut)
        return (420.0f + _log10f((x + 0.01f) / 0.19f) * 261.5f) / 1023.0f;
    else
        return (x * (171.2102946929f - 95.0f) / 0.01125f + 95.0f) / 1023.0f;
}

__DEVICE__ float S_lin_to_acescct(float x) {
    const float cut = 0.0078125f;
    const float slope = 10.5402377416545f;
    const float offset = 0.0729055341958355f;
    return (x > cut) ? (_log2f(x) + 9.72f) / 17.52f : slope * x + offset;
}

__DEVICE__ float S_lin_to_di(float x) {
    const float A = 0.0075f;
    const float B = 7.0f;
    const float C = 0.07329248f;
    const float M = 10.44426855f;
    const float LIN_CUT = 0.00262409f;
    return (x <= LIN_CUT) ? x * M : C * (_log2f(x + A) + B);
}

__DEVICE__ float S_lin_to_cineon(float x) {
    const float offset = 0.0108f;
    return (_log10f(x * (1.0f - offset) + offset) * 300.0f + 685.0f) / 1023.0f;
}

__DEVICE__ float S_lin_to_flog2(float x) {
    const float a = 5.555556f;
    const float b = 0.064829f;
    const float c = 0.245281f;
    const float d = 0.384316f;
    const float e = 8.799461f;
    const float f = 0.092864f;
    const float cut1 = 0.000889f;
    return (x >= cut1) ? c * _log10f(a * x + b) + d : e * x + f;
}

__DEVICE__ float S_lin_to_vlog(float x) {
    const float b = 0.00873f;
    const float c = 0.241514f;
    const float d = 0.598206f;
    const float cut1 = 0.01f;
    return (x < cut1) ? 5.6f * x + 0.125f : c * _log10f(x + b) + d;
}

__DEVICE__ float S_encode(float x, int tf) {
    switch (tf) {
        case 0: return S_lin_to_logc3(x);
        case 1: return S_lin_to_logc4(x);
        case 2: return S_lin_to_redlog3g10(x);
        case 3: return S_lin_to_slog3(x);
        case 4: return S_lin_to_acescct(x);
        case 5: return S_lin_to_di(x);
        case 6: return S_lin_to_cineon(x);
        case 7: return S_lin_to_flog2(x);
        case 8: return S_lin_to_vlog(x);
        default: return x;
    }
}
// ============================================================
// 5x7 bitmap glyph table
// Each row returns 5 bits (LSB = rightmost column).
// Used by chart overlays and the PrimeraSkin Show Mask legend.
// ============================================================

__DEVICE__ int S_glyph_row(int ch, int row) {
    switch (ch) {
        case '0': return (row==0||row==6)?31:27;
        case '1': return row==1?14:row==6?31:6;
        case '2': return (row==0||row==3||row==6)?31:row<3?3:24;
        case '3': return (row==0||row==3||row==6)?31:3;
        case '4': return row<=2?27:row==3?31:3;
        case '5': return (row==0||row==3||row==6)?31:row<3?24:3;
        case '6': return (row==0||row==3||row==6)?31:row<3?24:27;
        case '7': return row==0?31:row<=2?3:6;
        case '8': return (row==0||row==3||row==6)?31:27;
        case '9': return (row==0||row==3||row==6)?31:row<3?27:3;
        case 'A': return (row==0||row==3)?31:27;
        case 'C': return (row==0||row==6)?31:24;
        case 'D': return (row==0||row==6)?30:27;
        case 'E': return (row==0||row==3||row==6)?31:24;
        case 'F': return (row==0||row==3)?31:24;
        case 'G': return (row==0||row==6)?31:row<=2?24:27;
        case 'I': return (row==0||row==6)?31:6;
        case 'K': return row==0?17:row==1?18:row==2?20:row==3?24:row==4?20:row==5?18:17;
        case 'L': return row==6?31:24;
        case 'M': return row==0?17:row<=2?27:row==3?21:17;
        case 'N': return (row==0||row==6)?17:row<=2?25:row==3?21:19;
        case 'O': return (row==0||row==6)?31:27;
        case 'R': return (row==0||row==3)?31:row<=2?27:row==4?26:25;
        case 'S': return (row==0||row==3||row==6)?31:row<3?24:3;
        case 'T': return row==0?31:6;
        case 'Y': return row<=1?27:row==2?14:6;
        case '%': return row==0?25:row==1?26:row==2?2:row==3?4:row==4?8:row==5?19:3;
        case '=': return (row==2||row==4)?31:0;
        case '.': return row>=5?12:0;
        default:  return 0;
    }
}

// ============================================================
// Stop chart (renderer)
// Requires: S_encode(), S_glyph_row()
// ============================================================

__DEVICE__ int S_tf_name_char(int tf, int pos) {
    switch (tf) {
        case 0:
            if (pos==0) return 'L'; if (pos==1) return 'O'; if (pos==2) return 'G';
            if (pos==3) return 'C'; if (pos==4) return '3'; return 0;
        case 1:
            if (pos==0) return 'L'; if (pos==1) return 'O'; if (pos==2) return 'G';
            if (pos==3) return 'C'; if (pos==4) return '4'; return 0;
        case 2:
            if (pos==0) return 'R'; if (pos==1) return 'E'; if (pos==2) return 'D';
            if (pos==3) return 'L'; if (pos==4) return 'O'; if (pos==5) return 'G';
            if (pos==6) return '3'; if (pos==7) return 'G'; if (pos==8) return '1';
            if (pos==9) return '0'; return 0;
        case 3:
            if (pos==0) return 'S'; if (pos==1) return 'L'; if (pos==2) return 'O';
            if (pos==3) return 'G'; if (pos==4) return '3'; return 0;
        case 4:
            if (pos==0) return 'A'; if (pos==1) return 'C'; if (pos==2) return 'E';
            if (pos==3) return 'S'; if (pos==4) return 'C'; if (pos==5) return 'C';
            if (pos==6) return 'T'; return 0;
        case 5:
            if (pos==0) return 'D'; if (pos==1) return 'I'; return 0;
        case 6:
            if (pos==0) return 'C'; if (pos==1) return 'I'; if (pos==2) return 'N';
            if (pos==3) return 'E'; if (pos==4) return 'O'; if (pos==5) return 'N';
            return 0;
        case 7:
            if (pos==0) return 'F'; if (pos==1) return 'L'; if (pos==2) return 'O';
            if (pos==3) return 'G'; if (pos==4) return '2'; return 0;
        default: return 0;
    }
}

__DEVICE__ int S_tf_name_len(int tf) {
    switch (tf) {
        case 0: return 5;  case 1: return 5;  case 2: return 10;
        case 3: return 5;  case 4: return 7;  case 5: return 2;
        case 6: return 6;  case 7: return 5;  default: return 0;
    }
}

__DEVICE__ float3 S_render_chart(int p_Width, int p_Height, int p_X, int p_Y, int tf) {
    int px = p_X;
    int py = p_Y;
    float x_norm = (float)px / (float)p_Width;
    float y_norm = (float)py / (float)p_Height;

    // Upper half: 17 stop columns (-8 to +8)
    if (y_norm < 0.5f) {
        int num_cols = 17;
        int col = (int)(x_norm * (float)num_cols);
        if (col >= num_cols) col = num_cols - 1;
        int stop = col - 8;
        float lin = 0.18f * _powf(2.0f, (float)stop);
        float val = S_encode(lin, tf);
        val = _fmaxf(0.0f, _fminf(val, 1.0f));
        return make_float3(val, val, val);
    }

    // Lower half: gradient + text overlay
    float val = x_norm;
    float mid_grey = S_encode(0.18f, tf);

    int cell = p_Height / 100;
    if (cell < 2) cell = 2;

    int mg_d1 = (int)(mid_grey * 10.0f) % 10;
    int mg_d2 = (int)(mid_grey * 100.0f) % 10;
    int mg_d3 = (int)(mid_grey * 1000.0f + 0.5f) % 10;

    int char_w = 6 * cell;
    int char_h = 7 * cell;
    int line_gap = 3 * cell;
    int pad = 2 * cell;

    int name_len = S_tf_name_len(tf);
    int line2_len = 16;

    int lower_top = p_Height / 2;
    int lower_h = p_Height - lower_top;
    int total_text_h = char_h * 2 + line_gap;
    int line1_top = lower_top + (lower_h - total_text_h) / 2;
    int line2_top = line1_top + char_h + line_gap;

    int line1_left = (p_Width - name_len * char_w + cell) / 2;
    int line2_left = (p_Width - line2_len * char_w + cell) / 2;

    int box_l = (line1_left < line2_left ? line1_left : line2_left) - pad;
    int box_r = line1_left + name_len * char_w;
    int box_r2 = line2_left + line2_len * char_w;
    if (box_r2 > box_r) box_r = box_r2;
    box_r += pad;
    int box_t = line1_top - pad;
    int box_b = line2_top + char_h + pad;

    int in_box = (px >= box_l && px < box_r && py >= box_t && py < box_b) ? 1 : 0;
    int text_hit = 0;

    if (py >= line1_top && py < line1_top + char_h) {
        int lx = px - line1_left;
        if (lx >= 0 && lx < name_len * char_w) {
            int ci = lx / char_w;
            int cx = (lx % char_w) / cell;
            int cy = (py - line1_top) / cell;
            if (cx < 5) {
                int ch = S_tf_name_char(tf, ci);
                if (ch > 0) {
                    int mask = S_glyph_row(ch, cy);
                    if ((mask >> (4 - cx)) & 1) text_hit = 1;
                }
            }
        }
    }

    if (text_hit == 0 && py >= line2_top && py < line2_top + char_h) {
        int lx = px - line2_left;
        if (lx >= 0 && lx < line2_len * char_w) {
            int ci = lx / char_w;
            int cx = (lx % char_w) / cell;
            int cy = (py - line2_top) / cell;
            if (cx < 5) {
                int ch = 0;
                switch (ci) {
                    case 0: ch='1'; break; case 1: ch='8'; break;
                    case 2: ch='%'; break; case 3: ch=' '; break;
                    case 4: ch='G'; break; case 5: ch='R'; break;
                    case 6: ch='A'; break; case 7: ch='Y'; break;
                    case 8: ch=' '; break; case 9: ch='='; break;
                    case 10: ch=' '; break; case 11: ch='0'; break;
                    case 12: ch='.'; break;
                    case 13: ch='0'+mg_d1; break;
                    case 14: ch='0'+mg_d2; break;
                    case 15: ch='0'+mg_d3; break;
                }
                if (ch > 32) {
                    int mask = S_glyph_row(ch, cy);
                    if ((mask >> (4 - cx)) & 1) text_hit = 1;
                }
            }
        }
    }

    if (text_hit == 1) {
        val = mid_grey + 0.12f;
    } else if (in_box == 1) {
        val = mid_grey;
    }

    return make_float3(val, val, val);
}

// ============================================================
// Subtractive split toning
// ============================================================

__DEVICE__ float S_smooth_step(float edge0, float edge1, float x) {
    float t = _clampf((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

__DEVICE__ float S_calculate_subtractive_impact(float3 rgb_adj, float3 cmy_adj) {
    float total_reduction = 0.0f;

    total_reduction += _fmaxf(0.0f, rgb_adj.x) * 0.5f;
    total_reduction += _fmaxf(0.0f, rgb_adj.y) * 0.5f;
    total_reduction += _fmaxf(0.0f, rgb_adj.z) * 0.5f;

    total_reduction += _fmaxf(0.0f, -rgb_adj.x) * 0.33f;
    total_reduction += _fmaxf(0.0f, -rgb_adj.y) * 0.33f;
    total_reduction += _fmaxf(0.0f, -rgb_adj.z) * 0.33f;

    total_reduction += _fmaxf(0.0f, cmy_adj.x) * 0.33f;
    total_reduction += _fmaxf(0.0f, cmy_adj.y) * 0.33f;
    total_reduction += _fmaxf(0.0f, cmy_adj.z) * 0.33f;

    total_reduction += _fmaxf(0.0f, -cmy_adj.x) * 0.5f;
    total_reduction += _fmaxf(0.0f, -cmy_adj.y) * 0.5f;
    total_reduction += _fmaxf(0.0f, -cmy_adj.z) * 0.5f;

    return total_reduction;
}

__DEVICE__ float S_calculate_luminance_compensation(float subtractive_impact) {
    if (subtractive_impact <= 0.0f) return 1.0f;
    float compensation = 1.0f + (subtractive_impact * 2.0f);
    return _clampf(compensation, 1.0f, 2.0f);
}

__DEVICE__ float3 S_apply_subtractive_adjustment(float3 rgb, float3 rgb_adjustment, float3 cmy_adjustment) {
    float3 result = rgb;

    if (rgb_adjustment.x > 0.0f) {
        result.y *= (1.0f - rgb_adjustment.x);
        result.z *= (1.0f - rgb_adjustment.x);
    } else if (rgb_adjustment.x < 0.0f) {
        result.x *= (1.0f + rgb_adjustment.x);
    }

    if (rgb_adjustment.y > 0.0f) {
        result.x *= (1.0f - rgb_adjustment.y);
        result.z *= (1.0f - rgb_adjustment.y);
    } else if (rgb_adjustment.y < 0.0f) {
        result.y *= (1.0f + rgb_adjustment.y);
    }

    if (rgb_adjustment.z > 0.0f) {
        result.x *= (1.0f - rgb_adjustment.z);
        result.y *= (1.0f - rgb_adjustment.z);
    } else if (rgb_adjustment.z < 0.0f) {
        result.z *= (1.0f + rgb_adjustment.z);
    }

    if (cmy_adjustment.x > 0.0f) {
        result.x *= (1.0f - cmy_adjustment.x);
    } else if (cmy_adjustment.x < 0.0f) {
        result.y *= (1.0f + cmy_adjustment.x);
        result.z *= (1.0f + cmy_adjustment.x);
    }

    if (cmy_adjustment.y > 0.0f) {
        result.y *= (1.0f - cmy_adjustment.y);
    } else if (cmy_adjustment.y < 0.0f) {
        result.x *= (1.0f + cmy_adjustment.y);
        result.z *= (1.0f + cmy_adjustment.y);
    }

    if (cmy_adjustment.z > 0.0f) {
        result.z *= (1.0f - cmy_adjustment.z);
    } else if (cmy_adjustment.z < 0.0f) {
        result.x *= (1.0f + cmy_adjustment.z);
        result.y *= (1.0f + cmy_adjustment.z);
    }

    return result;
}

__DEVICE__ float3 S_draw_curve_overlay(float3 rgb, int p_X, int p_Y, int p_Width, int p_Height, float pivot_val, float transition_val) {
    float x_norm = (float)p_X / (float)p_Width;
    float y_norm = 1.0f - (float)p_Y / (float)p_Height;

    float shadow_edge = pivot_val - transition_val * 0.5f;
    float highlight_edge = pivot_val + transition_val * 0.5f;

    float shadow_weight = 1.0f - S_smooth_step(shadow_edge, highlight_edge, x_norm);
    float highlight_weight = S_smooth_step(shadow_edge, highlight_edge, x_norm);

    float curve_thickness = 0.003f;

    float shadow_curve_y = 0.7f + shadow_weight * 0.2f;
    if (_fabs(y_norm - shadow_curve_y) < curve_thickness) {
        rgb = _mix(rgb, make_float3(1.0f, 0.3f, 0.3f), 0.7f);
    }

    float highlight_curve_y = 0.3f + highlight_weight * 0.2f;
    if (_fabs(y_norm - highlight_curve_y) < curve_thickness) {
        rgb = _mix(rgb, make_float3(0.3f, 0.5f, 1.0f), 0.7f);
    }

    float pivot_x = pivot_val;
    if (_fabs(x_norm - pivot_x) < curve_thickness) {
        rgb = _mix(rgb, make_float3(1.0f, 1.0f, 0.3f), 0.5f);
    }

    return rgb;
}

// ============================================================
// Main Transform
// ============================================================

__DEVICE__ float3 S_transform(PrimeraParams u, int p_Width, int p_Height, int p_X, int p_Y, float p_R, float p_G, float p_B) {

    float effective_pivot = S_encode(0.18f, 5) + u.S_pivot;

    if (u.S_p_show_chart == 1) {
        float3 chart = S_render_chart(p_Width, p_Height, p_X, p_Y, 5);
        return chart;
    }

    float3 rgb = make_float3(p_R, p_G, p_B);

    float width = (float)p_Width;
    float height = (float)p_Height;
    float X = (float)p_X;
    float Y = height - (float)p_Y;
    float3 ramp = make_float3(X / width, X / width, X / width);

    float luma = S_luminance_dwg(rgb);

    float shadow_edge = effective_pivot - u.S_transition * 0.5f;
    float highlight_edge = effective_pivot + u.S_transition * 0.5f;

    float shadow_weight = 1.0f - S_smooth_step(shadow_edge, highlight_edge, luma);
    float highlight_weight = S_smooth_step(shadow_edge, highlight_edge, luma);

    float3 shadow_rgb_adj = make_float3(u.S_shadow_red, u.S_shadow_green, u.S_shadow_blue);
    float3 shadow_cmy_adj = make_float3(u.S_shadow_cyan, u.S_shadow_magenta, u.S_shadow_yellow);
    float3 highlight_rgb_adj = make_float3(u.S_highlight_red, u.S_highlight_green, u.S_highlight_blue);
    float3 highlight_cmy_adj = make_float3(u.S_highlight_cyan, u.S_highlight_magenta, u.S_highlight_yellow);

    float3 final_shadow_rgb = shadow_rgb_adj * shadow_weight;
    float3 final_shadow_cmy = shadow_cmy_adj * shadow_weight;
    float3 final_highlight_rgb = highlight_rgb_adj * highlight_weight;
    float3 final_highlight_cmy = highlight_cmy_adj * highlight_weight;

    float3 combined_rgb_adj = final_shadow_rgb + final_highlight_rgb;
    float3 combined_cmy_adj = final_shadow_cmy + final_highlight_cmy;

    rgb = S_apply_subtractive_adjustment(rgb, combined_rgb_adj, combined_cmy_adj);

    if (u.S_preserve_luminance > 0.0f) {
        float subtractive_impact = S_calculate_subtractive_impact(combined_rgb_adj, combined_cmy_adj);
        float compensation = S_calculate_luminance_compensation(subtractive_impact);
        rgb *= 1.0f + (compensation - 1.0f) * u.S_preserve_luminance;
    }

    float ramp_luma = X / width;
    float ramp_shadow_weight = 1.0f - S_smooth_step(shadow_edge, highlight_edge, ramp_luma);
    float ramp_highlight_weight = S_smooth_step(shadow_edge, highlight_edge, ramp_luma);

    float3 ramp_shadow_rgb = shadow_rgb_adj * ramp_shadow_weight;
    float3 ramp_shadow_cmy = shadow_cmy_adj * ramp_shadow_weight;
    float3 ramp_highlight_rgb = highlight_rgb_adj * ramp_highlight_weight;
    float3 ramp_highlight_cmy = highlight_cmy_adj * ramp_highlight_weight;

    float3 ramp_combined_rgb_adj = ramp_shadow_rgb + ramp_highlight_rgb;
    float3 ramp_combined_cmy_adj = ramp_shadow_cmy + ramp_highlight_cmy;

    ramp = S_apply_subtractive_adjustment(ramp, ramp_combined_rgb_adj, ramp_combined_cmy_adj);

    if (u.S_preserve_luminance > 0.0f) {
        float ramp_subtractive_impact = S_calculate_subtractive_impact(ramp_combined_rgb_adj, ramp_combined_cmy_adj);
        float ramp_compensation = S_calculate_luminance_compensation(ramp_subtractive_impact);
        ramp *= 1.0f + (ramp_compensation - 1.0f) * u.S_preserve_luminance;
    }

    if (u.S_visualize) {
        rgb = S_draw_curve_overlay(rgb, p_X, p_Y, p_Width, p_Height, effective_pivot, u.S_transition);
    }

    if (u.S_opt_showcurve == 1) {
        float overlayR = ramp.x >= (Y - 5.0f) / height && ramp.x <= (Y + 5.0f) / height ? 1.0f : 0.0f;
        float overlayG = ramp.y >= (Y - 5.0f) / height && ramp.y <= (Y + 5.0f) / height ? 1.0f : 0.0f;
        float overlayB = ramp.z >= (Y - 5.0f) / height && ramp.z <= (Y + 5.0f) / height ? 1.0f : 0.0f;

        rgb.x = overlayR == 0.0f ? rgb.x : overlayR;
        rgb.y = overlayG == 0.0f ? rgb.y : overlayG;
        rgb.z = overlayB == 0.0f ? rgb.z : overlayB;
    }

    if (u.S_opt_showramp == 1) {
        float y_norm = 1.0f - (float)p_Y / (float)p_Height;
        if (y_norm >= u.S_ramp_pos && y_norm <= u.S_ramp_pos + 0.15f) {
            rgb.x = ramp.x;
            rgb.y = ramp.y;
            rgb.z = ramp.z;
        }
    }

    return rgb;
}
__DEVICE__ float3 upstream(PrimeraParams u, int w, int h, int x, int y, float3 c) {
    if (u.enable_P) c = P_transform(u, w, h, x, y, c.x, c.y, c.z);
    if (u.enable_H) c = H_transform(u, w, h, x, y, c.x, c.y, c.z);
    if (u.enable_S) c = S_transform(u, w, h, x, y, c.x, c.y, c.z);
    return c;
}
// PrimeraSkin v0.6.0
// https://github.com/geoffsmithBK/primera-suite

// ============================================================
// UI Parameters
// ============================================================


// ============================================================
// HSV conversion
// ============================================================

__DEVICE__ float3 K_rgb_to_hsv(float3 rgb) {
    float r = rgb.x, g = rgb.y, b = rgb.z;
    float cmax = _fmaxf(r, _fmaxf(g, b));
    float cmin = _fminf(r, _fminf(g, b));
    float delta = cmax - cmin;

    float h = 0.0f;
    if (delta > 0.0f) {
        if (cmax == r)      h = (g - b) / delta;
        else if (cmax == g) h = (b - r) / delta + 2.0f;
        else                h = (r - g) / delta + 4.0f;
        if (h < 0.0f) h += 6.0f;
        h /= 6.0f;
    }

    float s = (cmax > 0.0f) ? delta / cmax : 0.0f;
    return make_float3(h, s, cmax);
}

__DEVICE__ float3 K_hsv_to_rgb(float3 hsv) {
    float h = hsv.x * 6.0f;
    float s = hsv.y;
    float v = hsv.z;

    float c = v * s;
    float x = c * (1.0f - _fabs(h - 2.0f * _floorf(h * 0.5f) - 1.0f));
    float m = v - c;

    float r, g, b;
    if      (h < 1.0f) { r = c; g = x; b = 0.0f; }
    else if (h < 2.0f) { r = x; g = c; b = 0.0f; }
    else if (h < 3.0f) { r = 0.0f; g = c; b = x; }
    else if (h < 4.0f) { r = 0.0f; g = x; b = c; }
    else if (h < 5.0f) { r = x; g = 0.0f; b = c; }
    else               { r = c; g = 0.0f; b = x; }

    return make_float3(r + m, g + m, b + m);
}

// ============================================================
// Skintone protection
// Requires: K_rgb_to_hsv()
// ============================================================

#define K_SKIN_HUE_CENTER (28.0f / 360.0f)
#define K_SKIN_HUE_WIDTH  (28.0f / 360.0f)

__DEVICE__ float K_hue_distance(float h1, float h2) {
    float d = _fabs(h1 - h2);
    return _fminf(d, 1.0f - d);
}

__DEVICE__ float K_smoothstep(float edge0, float edge1, float x) {
    float t = _clampf((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

__DEVICE__ float K_skin_mask_range(float r, float g, float b, float range) {
    float3 hsv = K_rgb_to_hsv(make_float3(r, g, b));
    float h = hsv.x;
    float s = hsv.y;

    if (s < 1e-6f || hsv.z < 1e-6f) return 0.0f;

    float width = K_SKIN_HUE_WIDTH * range;
    float dist = K_hue_distance(h, K_SKIN_HUE_CENTER);
    float hue_gate = 1.0f - K_smoothstep(0.0f, width, dist);
    float sat_gate = K_smoothstep(0.08f, 0.35f, s);

    return hue_gate * sat_gate;
}

__DEVICE__ float K_skin_mask(float r, float g, float b) {
    return K_skin_mask_range(r, g, b, 1.0f);
}

// Detection uses rg-chromaticity (normalized RGB ratios) rather than HSV
// saturation. Ratios are invariant to overall brightness, so the mask catches
// fair and dark skin equally well. Log-space skin clusters tightly around
// gn ≈ 0.33 with rn > bn.
__DEVICE__ float K_skin_mask_gated(float r, float g, float b, float range, float lo, float hi) {
    float sum = r + g + b;
    if (sum < 0.01f) return 0.0f;

    float rn = r / sum;
    float gn = g / sum;
    float bn = b / sum;

    // Skin chromaticity: gn sits in a narrow band, rn is red-leading vs bn.
    // Ramp widths are deliberately wide to match the smoothness of the
    // previous HSV-sat approach — narrow ramps make Density slider blotchy.
    float gn_gate = K_smoothstep(0.22f, 0.33f, gn) * (1.0f - K_smoothstep(0.37f, 0.48f, gn));
    float rb_gate = K_smoothstep(0.0f, 0.04f, rn - bn);
    float chroma_gate = gn_gate * rb_gate;

    // Hue gate preserves user-facing Range control (narrows/widens the pie slice)
    float3 hsv = K_rgb_to_hsv(make_float3(r, g, b));
    float h = hsv.x;
    float v = hsv.z;

    float width = K_SKIN_HUE_WIDTH * range;
    float dist = K_hue_distance(h, K_SKIN_HUE_CENTER);
    float hue_gate = 1.0f - K_smoothstep(0.0f, width, dist);

    // Value gates (user-facing Low/High Gate sliders)
    float lo_gate = (lo > 0.001f) ? K_smoothstep(lo * 0.5f, lo, v) : 1.0f;
    float hi_gate = (hi < 0.999f) ? 1.0f - K_smoothstep(hi - (1.0f - hi) * 0.5f, hi, v) : 1.0f;

    return hue_gate * chroma_gate * lo_gate * hi_gate;
}

// ============================================================
// Soft gamut squeeze (tanh shoulder + exponential toe)
// ============================================================

__DEVICE__ float3 K_soft_squeeze(float3 rgb) {
    float r = rgb.x, g = rgb.y, b = rgb.z;

    const float knee = 0.9f;
    const float range = 0.1f;
    if (r > knee) { float t = (r - knee) / range; r = knee + range * _tanhf(t); }
    if (g > knee) { float t = (g - knee) / range; g = knee + range * _tanhf(t); }
    if (b > knee) { float t = (b - knee) / range; b = knee + range * _tanhf(t); }

    const float toe = 0.1f;
    if (r < toe) r = toe * _expf(r / toe - 1.0f);
    if (g < toe) g = toe * _expf(g / toe - 1.0f);
    if (b < toe) b = toe * _expf(b / toe - 1.0f);

    return make_float3(r, g, b);
}

// ============================================================
// 5x7 bitmap glyph table
// Each row returns 5 bits (LSB = rightmost column).
// Used by chart overlays and the PrimeraSkin Show Mask legend.
// ============================================================

__DEVICE__ int K_glyph_row(int ch, int row) {
    switch (ch) {
        case '0': return (row==0||row==6)?31:27;
        case '1': return row==1?14:row==6?31:6;
        case '2': return (row==0||row==3||row==6)?31:row<3?3:24;
        case '3': return (row==0||row==3||row==6)?31:3;
        case '4': return row<=2?27:row==3?31:3;
        case '5': return (row==0||row==3||row==6)?31:row<3?24:3;
        case '6': return (row==0||row==3||row==6)?31:row<3?24:27;
        case '7': return row==0?31:row<=2?3:6;
        case '8': return (row==0||row==3||row==6)?31:27;
        case '9': return (row==0||row==3||row==6)?31:row<3?27:3;
        case 'A': return (row==0||row==3)?31:27;
        case 'C': return (row==0||row==6)?31:24;
        case 'D': return (row==0||row==6)?30:27;
        case 'E': return (row==0||row==3||row==6)?31:24;
        case 'F': return (row==0||row==3)?31:24;
        case 'G': return (row==0||row==6)?31:row<=2?24:27;
        case 'I': return (row==0||row==6)?31:6;
        case 'K': return row==0?17:row==1?18:row==2?20:row==3?24:row==4?20:row==5?18:17;
        case 'L': return row==6?31:24;
        case 'M': return row==0?17:row<=2?27:row==3?21:17;
        case 'N': return (row==0||row==6)?17:row<=2?25:row==3?21:19;
        case 'O': return (row==0||row==6)?31:27;
        case 'R': return (row==0||row==3)?31:row<=2?27:row==4?26:25;
        case 'S': return (row==0||row==3||row==6)?31:row<3?24:3;
        case 'T': return row==0?31:6;
        case 'Y': return row<=1?27:row==2?14:6;
        case '%': return row==0?25:row==1?26:row==2?2:row==3?4:row==4?8:row==5?19:3;
        case '=': return (row==2||row==4)?31:0;
        case '.': return row>=5?12:0;
        default:  return 0;
    }
}

// ============================================================
// Transform
// ============================================================

#define K_COMPRESS_EXTRA 0.4f
#define K_LEGEND_STRIP_H 0.075f

// Representative skin chromaticity (R-leading, B-trailing). Used as the
// reference base for legend chip rendering so the strip reflects what the
// overlay actually produces on real skin rather than on uniform grey.
#define K_SKIN_BASE_R 0.55f
#define K_SKIN_BASE_G 0.42f
#define K_SKIN_BASE_B 0.25f

#define K_GLYPH_GREY 0.15f

// Separation (see color-contrast.MD): non-skin hues migrate toward the band
// opposite the skin vector. The target is derived, not user-facing.
#define K_SEP_COMPLEMENT      (K_SKIN_HUE_CENTER + 0.5f)   // ~208 deg (teal)
#define K_SEP_FIELD_GAIN      0.075f   // 27 deg max shift at the 90-deg flanks: ~30% migration cap
#define K_SEP_FALLBACK_SOFTEN 0.5f     // MINIMUM pooling strength for the Separation gate

// Continuous mapping from zone position (-1..+1) to chromaticity-shifted RGB.
// zone=-1: cyan/green side   zone=0: skin centre (gold)   zone=+1: magenta side
// Used by both the Show Mask overlay (per-pixel) and the Legend strip
// (gradient across x). Same gains drive both, so the strip is a closed-form
// rendering of the overlay function applied to a fixed skin base.
__DEVICE__ float3 K_zone_color(float zone, float3 base) {
    if (zone < -1.0f) zone = -1.0f;
    if (zone > 1.0f) zone = 1.0f;

    float gr, gg, gb;
    if (zone <= 0.0f) {
        float t = -zone;
        gr = 1.4f;
        gg = 1.3f - t * 0.8f;
        gb = 0.5f + t * 0.8f;
    } else {
        float t = zone;
        gr = 1.4f - t * 0.9f;
        gg = 1.3f;
        gb = 0.5f + t * 0.8f;
    }

    return make_float3(base.x * gr, base.y * gg, base.z * gb);
}

// ------------------------------------------------------------
// Spatial mask pooling (Soften)
// ------------------------------------------------------------
// The per-pixel chromaticity mask (K_skin_mask_gated) is a pure function of one
// pixel's RGB, so sensor grain dithers borderline skin pixels across the gate
// edges — leaving a stochastic "salt" of under-adjusted pixels inside solid
// skin (the Density slider's pointillist look). The fix is spatial: pool the
// mask over a sparse neighbourhood with a loose bilateral (chroma-similarity)
// weight, then SOFT-UNION the pooled value with the pixel's own mask so pooling
// only FILLS holes and never erodes confident edges.
//
// This helper lives here (not in skintone.dctlf) because that fragment is
// shared with PrimeraHue, which keeps the scalar K_transform signature and must
// not gain texture dependencies.

// Sparse tap pattern: centre + 3 concentric rings (8/12/16 taps) at r/3, 2r/3,
// r. Ring angles are golden-angle staggered (137.5°) so taps never line up on
// image axes (avoids axis-aligned banding). cos/sin computed per tap in fixed
// loops — fixed-bound, so acceptable in DCTL.
#define K_POOL_GOLDEN 2.39996323f   // golden angle in radians (~137.5°)

__DEVICE__ float K_pool_skin_mask(PrimeraParams u, __TEXTURE__ p_TexR, __TEXTURE__ p_TexG, __TEXTURE__ p_TexB,
                                int p_Width, int p_Height, int p_X, int p_Y,
                                float r_c, float g_c, float b_c,
                                float m_own, float radius_px,
                                float range, float lo, float hi) {
    // Centre chroma reference in rg-chromaticity space.
    float sum_c = r_c + g_c + b_c;
    float gn_c, dn_c;
    int center_valid = 0;
    if (sum_c >= 0.01f) {
        gn_c = g_c / sum_c;
        dn_c = (r_c - b_c) / sum_c;   // rn - bn
        center_valid = 1;
    } else {
        gn_c = 0.0f;
        dn_c = 0.0f;
    }

    const float sigma_s = radius_px * 0.5f;          // spatial Gaussian sigma
    const float inv2ss = 1.0f / (2.0f * sigma_s * sigma_s);
    const float sigma_c = 0.06f;                     // chroma-similarity sigma
    const float inv2sc = 1.0f / (2.0f * sigma_c * sigma_c);

    // Accumulate centre tap first.
    float wsum = 1.0f;          // centre spatial gaussian = exp(0) = 1
    float msum = m_own;         // centre similarity = 1, mask = m_own

    // Ring configuration: radii fractions and tap counts.
    const int   ring_counts[3] = { 8, 12, 16 };
    const float ring_fracs[3]  = { 0.3333333f, 0.6666667f, 1.0f };
    // Per-ring starting phase, staggered by the golden angle so rings don't
    // share spoke directions.
    float phase = 0.0f;

    for (int ri = 0; ri < 3; ++ri) {
        int   n  = ring_counts[ri];
        float rr = radius_px * ring_fracs[ri];
        float dist2 = rr * rr;
        float w_spatial = _expf(-dist2 * inv2ss);
        float dtheta = 6.2831853f / (float)n;   // 2*pi / n

        for (int ti = 0; ti < 16; ++ti) {       // fixed upper bound (max ring)
            if (ti >= n) break;
            float theta = phase + dtheta * (float)ti;
            float ox = _cosf(theta) * rr;
            float oy = _sinf(theta) * rr;

            int sx = (int)(p_X + ox + 0.5f);
            int sy = (int)(p_Y + oy + 0.5f);
            if (sx < 0) sx = 0;
            if (sx > p_Width - 1) sx = p_Width - 1;
            if (sy < 0) sy = 0;
            if (sy > p_Height - 1) sy = p_Height - 1;

            float3 tap = upstream(u, p_Width, p_Height, sx, sy, make_float3(_tex2D(p_TexR, sx, sy), _tex2D(p_TexG, sx, sy), _tex2D(p_TexB, sx, sy)));
            float tr = tap.x, tg = tap.y, tb = tap.z;

            float tsum = tr + tg + tb;
            float w_sim;
            float m_tap;
            if (tsum < 0.01f || center_valid == 0) {
                // Degenerate tap (or degenerate centre): treat as zero.
                w_sim = 0.0f;
                m_tap = 0.0f;
            } else {
                float gn_t = tg / tsum;
                float dn_t = (tr - tb) / tsum;
                float dgn = gn_t - gn_c;
                float ddn = dn_t - dn_c;
                w_sim = _expf(-(dgn * dgn + ddn * ddn) * inv2sc);
                m_tap = K_skin_mask_gated(tr, tg, tb, range, lo, hi);
            }

            float w = w_spatial * w_sim;
            wsum += w;
            msum += w * m_tap;
        }

        phase += K_POOL_GOLDEN;
    }

    // wsum >= 1 always (centre tap has weight 1), so no zero-divide guard needed.
    return msum / wsum;
}

// Resolve's parser requires the K_transform() signature on a single line.
__DEVICE__ float3 K_transform(PrimeraParams u, int p_Width, int p_Height, int p_X, int p_Y, __TEXTURE__ p_TexR, __TEXTURE__ p_TexG, __TEXTURE__ p_TexB) {

    // Center pixel sampled once. These replace the old p_R/p_G/p_B working set.
    float3 center = upstream(u, p_Width, p_Height, p_X, p_Y, make_float3(_tex2D(p_TexR, p_X, p_Y), _tex2D(p_TexG, p_X, p_Y), _tex2D(p_TexB, p_X, p_Y)));
    float p_R = center.x, p_G = center.y, p_B = center.z;

    // Stable originals for Show Mask zone classification (must survive edits).
    float orig_r = p_R, orig_g = p_G, orig_b = p_B;

    float r = p_R, g = p_G, b = p_B;

    // --- Evenness: hue compression toward skin center ---
    // Uses a broader mask than other adjustments to catch outlier hues.
    // Deliberately UNPOOLED: Evenness is hue compression, not the source of the
    // Density graininess complaint, and pooling it would double the tap cost.
    if (u.K_p_evenness > 0.0f) {
        float cmask = K_skin_mask_gated(r, g, b, u.K_p_range + K_COMPRESS_EXTRA, u.K_p_lo_gate, u.K_p_hi_gate);

        if (cmask > 1e-6f) {
            float3 hsv = K_rgb_to_hsv(make_float3(r, g, b));

            float d = hsv.x - K_SKIN_HUE_CENTER;
            if (d > 0.5f) d -= 1.0f;
            if (d < -0.5f) d += 1.0f;

            d *= (1.0f - u.K_p_evenness * cmask);
            hsv.x = K_SKIN_HUE_CENTER + d;
            if (hsv.x < 0.0f) hsv.x += 1.0f;
            if (hsv.x > 1.0f) hsv.x -= 1.0f;

            float3 rgb_new = K_hsv_to_rgb(hsv);
            r = rgb_new.x;
            g = rgb_new.y;
            b = rgb_new.z;
        }
    }

    // --- Main mask (with value gates) ---
    // Own (per-pixel) mask, computed on the (possibly Evenness-edited) pixel.
    float m_own = K_skin_mask_gated(r, g, b, u.K_p_range, u.K_p_lo_gate, u.K_p_hi_gate);
    float mask = m_own;

    // --- Spatial pooling (Soften) ---
    // When Soften == 0 we skip pooling entirely: no taps, behaviour is
    // bit-identical to the per-pixel tool. The pooled value is soft-UNIONed
    // with the own mask so it can only fill holes, never erode edges.
    // m_pooled stays in scope: Show Mask reuses it rather than re-pooling.
    float m_pooled = 0.0f;
    if (u.K_p_soften > 0.0f) {
        float radius_px = u.K_p_soften * (float)p_Width * 0.004f;   // ~15px @ UHD
        m_pooled = K_pool_skin_mask(u, p_TexR, p_TexG, p_TexB,
                                  p_Width, p_Height, p_X, p_Y,
                                  r, g, b, m_own, radius_px,
                                  u.K_p_range, u.K_p_lo_gate, u.K_p_hi_gate);
        mask = 1.0f - (1.0f - m_own) * (1.0f - m_pooled);
    }

    // --- Separation gate mask ---
    // Separation INVERTS the mask, so its gate must come from a POOLED mask
    // (color-contrast.MD §3): inverting the raw per-pixel mask would not merely
    // skip grain-dithered borderline skin pixels, it would actively push them
    // toward teal — cyan freckles, the Density graininess bug weaponized.
    //
    // Soften's own radius cannot be trusted to supply that pooling: at
    // Soften = 0.01 the radius is sub-pixel (~0.15px @ UHD), every ring tap
    // rounds back onto the centre, and the gate silently degrades to the raw
    // per-pixel mask — i.e. nudging Soften off zero would WEAKEN the guard.
    // So the gate always pools at >= K_SEP_FALLBACK_SOFTEN, and reuses the main
    // pooled mask only once Soften has met that floor.
    //
    // Computed HERE, before the HSV adjustments, so the pool's centre chroma
    // reference is the same untouched pixel its taps read out of the texture.
    // `mask` is never overwritten, so Separation cannot perturb the skin-side
    // adjustments in either direction.
    float sep_mask = mask;
    if (u.K_p_separation > 0.0f && u.K_p_soften < K_SEP_FALLBACK_SOFTEN) {
        float sep_radius = K_SEP_FALLBACK_SOFTEN * (float)p_Width * 0.004f;
        float m_sep = K_pool_skin_mask(u, p_TexR, p_TexG, p_TexB,
                                     p_Width, p_Height, p_X, p_Y,
                                     r, g, b, m_own, sep_radius,
                                     u.K_p_range, u.K_p_lo_gate, u.K_p_hi_gate);
        sep_mask = 1.0f - (1.0f - m_own) * (1.0f - m_sep);
    }

    // Early exit if nothing to do and no overlays to render.
    // NOTE: this fires on the POOLED mask, so a pixel whose own mask is ~0 but
    // which receives a nonzero pooled adjustment is NOT skipped. Separation
    // targets exactly the mask≈0 pixels, so it must also hold the exit open.
    //
    // PERF: holding it open is not free, and Separation's cost is paid on the
    // BACKGROUND, not on skin. Soften == 0 normally means "no taps anywhere,
    // bit-identical to the per-pixel tool"; raising Separation revokes that,
    // because every background pixel now both misses this early exit and runs
    // the 36-tap gate pool above (108 texture fetches/px). In the band
    // 0 < Soften < K_SEP_FALLBACK_SOFTEN the frame pays for two independent
    // pools; at Soften >= K_SEP_FALLBACK_SOFTEN it is back to one. Judge
    // playback performance in Resolve with Separation at its intended dose —
    // benchmarking at Separation = 0 measures a different shader.
    if (mask < 1e-6f && u.K_p_separation <= 0.0f && u.K_p_show_mask == 0 && u.K_p_legend == 0)
        return make_float3(r, g, b);

    // --- Hue, Saturation, Density adjustments in HSV ---
    if (mask > 1e-6f) {
        float3 hsv = K_rgb_to_hsv(make_float3(r, g, b));

        // Hue rotation (slider direction matches legend: left = cyan, right = magenta)
        if (u.K_p_hue != 0.0f) {
            hsv.x -= mask * u.K_p_hue / 360.0f;
            if (hsv.x < 0.0f) hsv.x += 1.0f;
            if (hsv.x > 1.0f) hsv.x -= 1.0f;
        }

        // Saturation scale
        if (u.K_p_saturation != 0.0f) {
            hsv.y = _fmaxf(0.0f, hsv.y * (1.0f + mask * u.K_p_saturation));
        }

        // Density (value scale, positive = darker)
        if (u.K_p_density != 0.0f) {
            hsv.z = _fmaxf(0.0f, hsv.z * (1.0f - mask * u.K_p_density));
        }

        float3 adj = K_hsv_to_rgb(hsv);
        r = adj.x;
        g = adj.y;
        b = adj.z;
    }

    // --- Legend strip at bottom of frame (independent of Show Mask) ---
    // Resolve DCTL uses top-down pixel coords: p_Y=0 is the top row.
    // The bottom 7.5% of the frame becomes the strip.
    if (u.K_p_legend == 1) {
        float y_norm = (float)p_Y / (float)p_Height;
        if (y_norm >= 1.0f - K_LEGEND_STRIP_H) {
            float x_norm = (float)p_X / (float)p_Width;

            // Continuous gradient: x maps linearly to zone in [+1, -1] so the
            // strip reads left→right as GREEN → SKIN → MAG, matching the
            // labels and the Hue slider convention (left = green, right = mag).
            float zone_strip = 1.0f - x_norm * 2.0f;
            float3 skin_base = make_float3(K_SKIN_BASE_R, K_SKIN_BASE_G, K_SKIN_BASE_B);
            float3 chip = K_zone_color(zone_strip, skin_base);
            float lr = chip.x;
            float lg = chip.y;
            float lb = chip.z;

            int cell = p_Height / 100;
            if (cell < 2) cell = 2;
            int char_w = 6 * cell;
            int char_h = 7 * cell;

            int strip_top = (int)((1.0f - K_LEGEND_STRIP_H) * (float)p_Height);
            int strip_height = p_Height - strip_top;
            int block_w = p_Width / 3;
            int third = (int)(x_norm * 3.0f);
            if (third > 2) third = 2;
            int block_left = third * block_w;

            int text_len = (third == 0) ? 5 : 4;   // GREEN | SKIN | MAG.
            int text_w = text_len * char_w;
            int text_left = block_left + (block_w - text_w) / 2;
            int text_top = strip_top + (strip_height - char_h) / 2;

            if (p_Y >= text_top && p_Y < text_top + char_h) {
                int lx = p_X - text_left;
                if (lx >= 0 && lx < text_w) {
                    int ci = lx / char_w;
                    int cx = (lx % char_w) / cell;
                    int cy = (p_Y - text_top) / cell;
                    if (cx < 5) {
                        int ch = 0;
                        if (third == 0) {
                            if (ci == 0) ch = 'G';
                            else if (ci == 1) ch = 'R';
                            else if (ci == 2) ch = 'E';
                            else if (ci == 3) ch = 'E';
                            else ch = 'N';
                        } else if (third == 1) {
                            if (ci == 0) ch = 'S';
                            else if (ci == 1) ch = 'K';
                            else if (ci == 2) ch = 'I';
                            else ch = 'N';
                        } else {
                            if (ci == 0) ch = 'M';
                            else if (ci == 1) ch = 'A';
                            else if (ci == 2) ch = 'G';
                            else ch = '.';
                        }

                        int gmask = K_glyph_row(ch, cy);
                        if ((gmask >> (4 - cx)) & 1) {
                            lr = K_GLYPH_GREY; lg = K_GLYPH_GREY; lb = K_GLYPH_GREY;
                        }
                    }
                }
            }

            float3 legend_px = make_float3(lr, lg, lb);
            return legend_px;
        }
    }

    // --- Show Mask: three-zone false-color overlay ---
    // Zone color from ORIGINAL hues (stable classification);
    // mask strength from ADJUSTED pixels (shows slider effect via intensity)
    if (u.K_p_show_mask == 1) {
        // vmask mirrors the adjustment path: own mask, soft-unioned with the
        // pooled mask from the main pass. Taps read the original texture
        // either way, so re-pooling here would only change the centre chroma
        // reference — a negligible difference for double the tap cost.
        // (m_pooled is 0.0 when Soften == 0, making the union a no-op.)
        float vown = K_skin_mask_gated(r, g, b, u.K_p_range, u.K_p_lo_gate, u.K_p_hi_gate);
        float vmask = 1.0f - (1.0f - vown) * (1.0f - m_pooled);
        float3 hsv_adj = K_rgb_to_hsv(make_float3(r, g, b));
        float grey = hsv_adj.z;

        // Zone from original pixel hue — stable regardless of Hue slider
        float3 hsv_orig = K_rgb_to_hsv(make_float3(orig_r, orig_g, orig_b));
        float hd = hsv_orig.x - K_SKIN_HUE_CENTER;
        if (hd > 0.5f) hd -= 1.0f;
        if (hd < -0.5f) hd += 1.0f;

        float width = K_SKIN_HUE_WIDTH * u.K_p_range;
        float zone = (width > 1e-6f) ? hd / width : 0.0f;

        // Per-pixel zone tint — same closed-form mapping the legend uses.
        // Base is the pixel's own greyscale (V), so the overlay rides on the
        // image's local luminance and reads as natural false-colour.
        float3 grey_base = make_float3(grey, grey, grey);
        float3 zc = K_zone_color(zone, grey_base);

        // Square vmask for display: compresses marginal matches toward grey,
        // keeping peak-skin pixels brightly saturated for clearer visual feedback.
        float vd = vmask * vmask;
        float mr = grey + vd * (zc.x - grey);
        float mg = grey + vd * (zc.y - grey);
        float mb = grey + vd * (zc.z - grey);

        // NOTE: this returns BEFORE Separation, deliberately — the overlay is a
        // readout of the skin mask, not a preview of the graded result, so it
        // shows no teal push. The cost is an evaluation blind spot: the
        // protective moat (color-contrast.MD §3, §8 step 3 — warm wood/beige
        // within pooling radius of a face getting partially exempted) has no
        // false-colour representation and can only be judged by eye on the
        // graded image. If that proves too blind, the honest fix is a separate
        // Show Separation readout of (1 - sep_mask), not overloading this one
        // with a second meaning.
        return make_float3(mr, mg, mb);
    }

    // --- Separation: push non-skin hues toward the skin complement ---
    // Inverse-mask color contrast (design + hazards: color-contrast.MD).
    // sep_mask is pooled up top — see the gate comment there for why the
    // pooling is load-bearing rather than merely convenient.
    if (u.K_p_separation > 0.0f) {
        float3 hsv = K_rgb_to_hsv(make_float3(r, g, b));

        // Chroma weight keeps neutrals neutral — the push redistributes
        // existing color, never invents it. The V ramp re-derives the
        // deep-black guard: K_skin_mask_gated returns 0 for sum < 0.01, which
        // would otherwise invert to a FULL push on near-blacks. sum < 0.01
        // implies V <= 0.01, so this ramp closes that hole exactly, not
        // approximately.
        float w = u.K_p_separation * (1.0f - sep_mask)
                * K_smoothstep(0.08f, 0.35f, hsv.y)
                * K_smoothstep(0.01f, 0.05f, hsv.z);

        if (w > 1e-6f) {
            // The field is evaluated on the pixel's NATIVE hue, not its
            // adjusted one. Hue and Evenness rotate masked pixels off 28°, and
            // the repeller has to stay nailed to real skin: read the adjusted
            // hue instead and a partially-masked edge pixel drifts out of the
            // field's null and takes a real push, demoting the doubly-redundant
            // protection of color-contrast.MD §2 to single. The magnitude is
            // small — the Hue rotation is itself mask-scaled, so the field
            // offset and the (1-mask) weight peak at opposite ends and the
            // product tops out near 2.4° at mask ≈ 0.5, Hue = ±20 — but
            // anchoring is free, exact, and does not assume the Hue slider
            // keeps its current ±20 range. For non-skin pixels nothing is
            // adjusted, so native == current and this is a no-op.
            float3 hsv_native = K_rgb_to_hsv(make_float3(orig_r, orig_g, orig_b));
            float h_field = (hsv_native.y > 1e-6f) ? hsv_native.x : hsv.x;

            // Sinusoidal hue field: stable attractor at the complement,
            // unstable equilibrium at the skin hue itself (zero push there —
            // skin is protected by the field even before the mask gate), and
            // smooth everywhere on the circle, so adjacent hues never tear
            // apart at a watershed. Note the field is at its MAXIMUM at the
            // 90° flanks (118° green, 298° magenta); the point of zero push is
            // the repeller at 28°, which is exactly where naive shortest-path
            // rotation would tear.
            float dh = h_field - K_SEP_COMPLEMENT;
            if (dh > 0.5f) dh -= 1.0f;
            if (dh < -0.5f) dh += 1.0f;

            // KNOWN AND UNMITIGATED (color-contrast.MD §4.3): reds sit on the
            // magenta flank and drift the long way round. Crimson at 355° takes
            // -14.7° on an unmasked, saturated pixel at Separation = 1, landing
            // near 340° — the "why is her dress purple" failure, arriving at
            // roughly half the maximum dose. No protection notch yet, and that
            // is a decision, not an oversight: a notch flattens the field for
            // warm practicals and skin-adjacent wood/beige that this effect is
            // supposed to move. Evaluate on real footage before adding one.
            hsv.x -= w * K_SEP_FIELD_GAIN * _sinf(dh * 6.2831853f);
            if (hsv.x < 0.0f) hsv.x += 1.0f;
            if (hsv.x > 1.0f) hsv.x -= 1.0f;

            // Pure hue rotation at constant S/V stays in gamut, so this
            // needs no help from the mask-scaled squeeze below.
            float3 sep_rgb = K_hsv_to_rgb(hsv);
            r = sep_rgb.x;
            g = sep_rgb.y;
            b = sep_rgb.z;
        }
    }

    // Gamut squeeze scaled by mask. The squeeze exists to contain adjustment
    // overshoot, and adjustment magnitude already scales with mask — so
    // faintly pooled-in pixels (background within pooling radius of skin)
    // must not receive the full shoulder/toe, or Soften paints a squeeze halo
    // around skin against bright/dark backgrounds. Gain of 4 reaches full
    // squeeze by mask 0.25, keeping core-skin behaviour unchanged.
    float3 squeezed = K_soft_squeeze(make_float3(r, g, b));
    float sq = _fminf(mask * 4.0f, 1.0f);
    float3 result = make_float3(r + sq * (squeezed.x - r),
                                g + sq * (squeezed.y - g),
                                b + sq * (squeezed.z - b));
    return result;
}
__DEVICE__ float3 transform(int p_Width, int p_Height, int p_X, int p_Y, __TEXTURE__ p_TexR, __TEXTURE__ p_TexG, __TEXTURE__ p_TexB) {
    float3 c = make_float3(_tex2D(p_TexR, p_X, p_Y), _tex2D(p_TexG, p_X, p_Y), _tex2D(p_TexB, p_X, p_Y));
    if (bypass) return c;
    PrimeraParams u;
    u.P_p_exposure = P_p_exposure;
    u.P_p_bp = P_p_bp;
    u.P_p_temp = P_p_temp;
    u.P_p_tint = P_p_tint;
    u.P_p_contrast = P_p_contrast;
    u.P_p_pivot_offset = P_p_pivot_offset;
    u.P_p_shadows = P_p_shadows;
    u.P_p_highlights = P_p_highlights;
    u.P_p_high_soft = P_p_high_soft;
    u.P_p_neg_sat = P_p_neg_sat;
    u.P_p_pos_sat = P_p_pos_sat;
    u.P_p_preserve_lum = P_p_preserve_lum;
    u.P_p_show_chart = P_p_show_chart;
    u.H_p_red_hue = H_p_red_hue;
    u.H_p_red_den = H_p_red_den;
    u.H_p_yel_hue = H_p_yel_hue;
    u.H_p_yel_den = H_p_yel_den;
    u.H_p_grn_hue = H_p_grn_hue;
    u.H_p_grn_den = H_p_grn_den;
    u.H_p_cyn_hue = H_p_cyn_hue;
    u.H_p_cyn_den = H_p_cyn_den;
    u.H_p_blu_hue = H_p_blu_hue;
    u.H_p_blu_den = H_p_blu_den;
    u.H_p_mag_hue = H_p_mag_hue;
    u.H_p_mag_den = H_p_mag_den;
    u.H_p_preserve_lum = H_p_preserve_lum;
    u.H_p_soft_squeeze = H_p_soft_squeeze;
    u.H_p_hard_squeeze = H_p_hard_squeeze;
    u.H_p_cinecolor = H_p_cinecolor;
    u.H_p_protect_skin = H_p_protect_skin;
    u.S_pivot = S_pivot;
    u.S_transition = S_transition;
    u.S_shadow_red = S_shadow_red;
    u.S_shadow_green = S_shadow_green;
    u.S_shadow_blue = S_shadow_blue;
    u.S_shadow_cyan = S_shadow_cyan;
    u.S_shadow_magenta = S_shadow_magenta;
    u.S_shadow_yellow = S_shadow_yellow;
    u.S_highlight_red = S_highlight_red;
    u.S_highlight_green = S_highlight_green;
    u.S_highlight_blue = S_highlight_blue;
    u.S_highlight_cyan = S_highlight_cyan;
    u.S_highlight_magenta = S_highlight_magenta;
    u.S_highlight_yellow = S_highlight_yellow;
    u.S_preserve_luminance = S_preserve_luminance;
    u.S_opt_showcurve = S_opt_showcurve;
    u.S_visualize = S_visualize;
    u.S_opt_showramp = S_opt_showramp;
    u.S_ramp_pos = S_ramp_pos;
    u.S_p_show_chart = S_p_show_chart;
    u.K_p_hue = K_p_hue;
    u.K_p_saturation = K_p_saturation;
    u.K_p_density = K_p_density;
    u.K_p_range = K_p_range;
    u.K_p_soften = K_p_soften;
    u.K_p_evenness = K_p_evenness;
    u.K_p_separation = K_p_separation;
    u.K_p_lo_gate = K_p_lo_gate;
    u.K_p_hi_gate = K_p_hi_gate;
    u.K_p_show_mask = K_p_show_mask;
    u.K_p_legend = K_p_legend;
    u.enable_P = enable_P;
    u.enable_H = enable_H;
    u.enable_S = enable_S;
    u.enable_K = enable_K;
    if (u.enable_K) return K_transform(u, p_Width, p_Height, p_X, p_Y, p_TexR, p_TexG, p_TexB);
    return upstream(u, p_Width, p_Height, p_X, p_Y, c);
}

}
namespace Primera {
// Primera v0.6.0
// https://github.com/geoffsmithBK/primera-suite

// ============================================================
// UI Parameters
// ===============================================================

int p_tf = 0;
float p_exposure = 0.0;
float p_bp = 0.0;
float p_temp = 0.0;
float p_tint = 0.0;
float p_contrast = 1.0;
float p_pivot_offset = 0.0;
float p_shadows = 0.0;
float p_highlights = 0.0;
float p_high_soft = 0.0;
float p_neg_sat = 1.0;
float p_pos_sat = 0.0;
int p_preserve_lum = 0;
int p_show_chart = 0;
// ============================================================
// Luminance
// ============================================================

__DEVICE__ float luminance(float r, float g, float b) {
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

__DEVICE__ float luminance_dwg(float3 rgb) {
    return 0.2627f * rgb.x + 0.6780f * rgb.y + 0.0593f * rgb.z;
}

// ============================================================
// HSV conversion
// ============================================================

__DEVICE__ float3 rgb_to_hsv(float3 rgb) {
    float r = rgb.x, g = rgb.y, b = rgb.z;
    float cmax = _fmaxf(r, _fmaxf(g, b));
    float cmin = _fminf(r, _fminf(g, b));
    float delta = cmax - cmin;

    float h = 0.0f;
    if (delta > 0.0f) {
        if (cmax == r)      h = (g - b) / delta;
        else if (cmax == g) h = (b - r) / delta + 2.0f;
        else                h = (r - g) / delta + 4.0f;
        if (h < 0.0f) h += 6.0f;
        h /= 6.0f;
    }

    float s = (cmax > 0.0f) ? delta / cmax : 0.0f;
    return make_float3(h, s, cmax);
}

__DEVICE__ float3 hsv_to_rgb(float3 hsv) {
    float h = hsv.x * 6.0f;
    float s = hsv.y;
    float v = hsv.z;

    float c = v * s;
    float x = c * (1.0f - _fabs(h - 2.0f * _floorf(h * 0.5f) - 1.0f));
    float m = v - c;

    float r, g, b;
    if      (h < 1.0f) { r = c; g = x; b = 0.0f; }
    else if (h < 2.0f) { r = x; g = c; b = 0.0f; }
    else if (h < 3.0f) { r = 0.0f; g = c; b = x; }
    else if (h < 4.0f) { r = 0.0f; g = x; b = c; }
    else if (h < 5.0f) { r = x; g = 0.0f; b = c; }
    else               { r = c; g = 0.0f; b = x; }

    return make_float3(r + m, g + m, b + m);
}
// ============================================================
// Transfer functions: linear -> log
// ============================================================

__DEVICE__ float lin_to_logc3(float x) {
    const float a = 5.555556f;
    const float b = 0.052272f;
    const float c = 0.247190f;
    const float d = 0.385537f;
    const float e = 5.367655f;
    const float f = 0.092809f;
    const float cut = 0.010591f;
    return (x > cut) ? c * _log10f(a * x + b) + d : e * x + f;
}

__DEVICE__ float lin_to_logc4(float x) {
    const float a = (262144.0f - 16.0f) / 117.45f;
    const float b = (1023.0f - 95.0f) / 1023.0f;
    const float c = 95.0f / 1023.0f;
    const float s = (7.0f * 0.6931471805599453f * _powf(2.0f, 7.0f - 14.0f * c / b)) / (a * b);
    const float t = (_powf(2.0f, 14.0f * (-c / b) + 6.0f) - 64.0f) / a;
    return (x >= t) ? (_log2f(a * x + 64.0f) - 6.0f) / 14.0f * b + c : (x - t) / s;
}

__DEVICE__ float lin_to_redlog3g10(float x) {
    const float a = 0.224282f;
    const float b = 155.975327f;
    const float c = 0.01f;
    const float g = 15.1927f;
    float xc = x + c;
    return (xc < 0.0f) ? xc * g : a * _log10f(xc * b + 1.0f);
}

__DEVICE__ float lin_to_slog3(float x) {
    const float cut = 0.01125f;
    if (x >= cut)
        return (420.0f + _log10f((x + 0.01f) / 0.19f) * 261.5f) / 1023.0f;
    else
        return (x * (171.2102946929f - 95.0f) / 0.01125f + 95.0f) / 1023.0f;
}

__DEVICE__ float lin_to_acescct(float x) {
    const float cut = 0.0078125f;
    const float slope = 10.5402377416545f;
    const float offset = 0.0729055341958355f;
    return (x > cut) ? (_log2f(x) + 9.72f) / 17.52f : slope * x + offset;
}

__DEVICE__ float lin_to_di(float x) {
    const float A = 0.0075f;
    const float B = 7.0f;
    const float C = 0.07329248f;
    const float M = 10.44426855f;
    const float LIN_CUT = 0.00262409f;
    return (x <= LIN_CUT) ? x * M : C * (_log2f(x + A) + B);
}

__DEVICE__ float lin_to_cineon(float x) {
    const float offset = 0.0108f;
    return (_log10f(x * (1.0f - offset) + offset) * 300.0f + 685.0f) / 1023.0f;
}

__DEVICE__ float lin_to_flog2(float x) {
    const float a = 5.555556f;
    const float b = 0.064829f;
    const float c = 0.245281f;
    const float d = 0.384316f;
    const float e = 8.799461f;
    const float f = 0.092864f;
    const float cut1 = 0.000889f;
    return (x >= cut1) ? c * _log10f(a * x + b) + d : e * x + f;
}

__DEVICE__ float lin_to_vlog(float x) {
    const float b = 0.00873f;
    const float c = 0.241514f;
    const float d = 0.598206f;
    const float cut1 = 0.01f;
    return (x < cut1) ? 5.6f * x + 0.125f : c * _log10f(x + b) + d;
}

__DEVICE__ float encode(float x, int tf) {
    switch (tf) {
        case 0: return lin_to_logc3(x);
        case 1: return lin_to_logc4(x);
        case 2: return lin_to_redlog3g10(x);
        case 3: return lin_to_slog3(x);
        case 4: return lin_to_acescct(x);
        case 5: return lin_to_di(x);
        case 6: return lin_to_cineon(x);
        case 7: return lin_to_flog2(x);
        case 8: return lin_to_vlog(x);
        default: return x;
    }
}// ============================================================
// Transfer functions: log -> linear
// ============================================================

__DEVICE__ float logc3_to_lin(float x) {
    const float a = 5.555556f;
    const float b = 0.052272f;
    const float c = 0.247190f;
    const float d = 0.385537f;
    const float e = 5.367655f;
    const float f = 0.092809f;
    const float cut = 0.010591f;
    const float ecf = e * cut + f;
    return (x > ecf) ? (_powf(10.0f, (x - d) / c) - b) / a : (x - f) / e;
}

__DEVICE__ float logc4_to_lin(float x) {
    const float a = (262144.0f - 16.0f) / 117.45f;
    const float b = (1023.0f - 95.0f) / 1023.0f;
    const float c = 95.0f / 1023.0f;
    const float s = (7.0f * 0.6931471805599453f * _powf(2.0f, 7.0f - 14.0f * c / b)) / (a * b);
    const float t = (_powf(2.0f, 14.0f * (-c / b) + 6.0f) - 64.0f) / a;
    return (x >= 0.0f) ? (_powf(2.0f, 14.0f * ((x - c) / b) + 6.0f) - 64.0f) / a : x * s + t;
}

__DEVICE__ float redlog3g10_to_lin(float y) {
    const float a = 0.224282f;
    const float b = 155.975327f;
    const float c = 0.01f;
    const float g = 15.1927f;
    return (y < 0.0f) ? (y / g) - c : (_powf(10.0f, y / a) - 1.0f) / b - c;
}

__DEVICE__ float slog3_to_lin(float x) {
    const float cut = 171.2102946929f / 1023.0f;
    if (x >= cut)
        return _powf(10.0f, (x * 1023.0f - 420.0f) / 261.5f) * 0.19f - 0.01f;
    else
        return (x * 1023.0f - 95.0f) * 0.01125f / (171.2102946929f - 95.0f);
}

__DEVICE__ float acescct_to_lin(float x) {
    const float cut = 0.155251141552511f;
    const float slope = 10.5402377416545f;
    const float offset = 0.0729055341958355f;
    return (x > cut) ? _powf(2.0f, x * 17.52f - 9.72f) : (x - offset) / slope;
}

__DEVICE__ float di_to_lin(float x) {
    const float A = 0.0075f;
    const float B = 7.0f;
    const float C = 0.07329248f;
    const float M = 10.44426855f;
    const float LOG_CUT = 0.02740668f;
    return (x <= LOG_CUT) ? x / M : _powf(2.0f, x / C - B) - A;
}

__DEVICE__ float cineon_to_lin(float x) {
    const float offset = 0.0108f;
    return (_powf(10.0f, (x * 1023.0f - 685.0f) / 300.0f) - offset) / (1.0f - offset);
}

__DEVICE__ float flog2_to_lin(float x) {
    const float a = 5.555556f;
    const float b = 0.064829f;
    const float c = 0.245281f;
    const float d = 0.384316f;
    const float e = 8.799461f;
    const float f = 0.092864f;
    const float cut2 = 0.100686685370811f;
    return (x >= cut2) ? (_powf(10.0f, (x - d) / c) / a) - (b / a) : (x - f) / e;
}

__DEVICE__ float vlog_to_lin(float x) {
    const float b = 0.00873f;
    const float c = 0.241514f;
    const float d = 0.598206f;
    const float cut2 = 0.181f;
    return (x < cut2) ? (x - 0.125f) / 5.6f : _powf(10.0f, (x - d) / c) - b;
}

__DEVICE__ float decode(float x, int tf) {
    switch (tf) {
        case 0: return logc3_to_lin(x);
        case 1: return logc4_to_lin(x);
        case 2: return redlog3g10_to_lin(x);
        case 3: return slog3_to_lin(x);
        case 4: return acescct_to_lin(x);
        case 5: return di_to_lin(x);
        case 6: return cineon_to_lin(x);
        case 7: return flog2_to_lin(x);
        case 8: return vlog_to_lin(x);
        default: return x;
    }
}
// ============================================================
// 5x7 bitmap glyph table
// Each row returns 5 bits (LSB = rightmost column).
// Used by chart overlays and the PrimeraSkin Show Mask legend.
// ============================================================

__DEVICE__ int glyph_row(int ch, int row) {
    switch (ch) {
        case '0': return (row==0||row==6)?31:27;
        case '1': return row==1?14:row==6?31:6;
        case '2': return (row==0||row==3||row==6)?31:row<3?3:24;
        case '3': return (row==0||row==3||row==6)?31:3;
        case '4': return row<=2?27:row==3?31:3;
        case '5': return (row==0||row==3||row==6)?31:row<3?24:3;
        case '6': return (row==0||row==3||row==6)?31:row<3?24:27;
        case '7': return row==0?31:row<=2?3:6;
        case '8': return (row==0||row==3||row==6)?31:27;
        case '9': return (row==0||row==3||row==6)?31:row<3?27:3;
        case 'A': return (row==0||row==3)?31:27;
        case 'C': return (row==0||row==6)?31:24;
        case 'D': return (row==0||row==6)?30:27;
        case 'E': return (row==0||row==3||row==6)?31:24;
        case 'F': return (row==0||row==3)?31:24;
        case 'G': return (row==0||row==6)?31:row<=2?24:27;
        case 'I': return (row==0||row==6)?31:6;
        case 'K': return row==0?17:row==1?18:row==2?20:row==3?24:row==4?20:row==5?18:17;
        case 'L': return row==6?31:24;
        case 'M': return row==0?17:row<=2?27:row==3?21:17;
        case 'N': return (row==0||row==6)?17:row<=2?25:row==3?21:19;
        case 'O': return (row==0||row==6)?31:27;
        case 'R': return (row==0||row==3)?31:row<=2?27:row==4?26:25;
        case 'S': return (row==0||row==3||row==6)?31:row<3?24:3;
        case 'T': return row==0?31:6;
        case 'Y': return row<=1?27:row==2?14:6;
        case '%': return row==0?25:row==1?26:row==2?2:row==3?4:row==4?8:row==5?19:3;
        case '=': return (row==2||row==4)?31:0;
        case '.': return row>=5?12:0;
        default:  return 0;
    }
}

// ============================================================
// Stop chart (renderer)
// Requires: encode(), glyph_row()
// ============================================================

__DEVICE__ int tf_name_char(int tf, int pos) {
    switch (tf) {
        case 0:
            if (pos==0) return 'L'; if (pos==1) return 'O'; if (pos==2) return 'G';
            if (pos==3) return 'C'; if (pos==4) return '3'; return 0;
        case 1:
            if (pos==0) return 'L'; if (pos==1) return 'O'; if (pos==2) return 'G';
            if (pos==3) return 'C'; if (pos==4) return '4'; return 0;
        case 2:
            if (pos==0) return 'R'; if (pos==1) return 'E'; if (pos==2) return 'D';
            if (pos==3) return 'L'; if (pos==4) return 'O'; if (pos==5) return 'G';
            if (pos==6) return '3'; if (pos==7) return 'G'; if (pos==8) return '1';
            if (pos==9) return '0'; return 0;
        case 3:
            if (pos==0) return 'S'; if (pos==1) return 'L'; if (pos==2) return 'O';
            if (pos==3) return 'G'; if (pos==4) return '3'; return 0;
        case 4:
            if (pos==0) return 'A'; if (pos==1) return 'C'; if (pos==2) return 'E';
            if (pos==3) return 'S'; if (pos==4) return 'C'; if (pos==5) return 'C';
            if (pos==6) return 'T'; return 0;
        case 5:
            if (pos==0) return 'D'; if (pos==1) return 'I'; return 0;
        case 6:
            if (pos==0) return 'C'; if (pos==1) return 'I'; if (pos==2) return 'N';
            if (pos==3) return 'E'; if (pos==4) return 'O'; if (pos==5) return 'N';
            return 0;
        case 7:
            if (pos==0) return 'F'; if (pos==1) return 'L'; if (pos==2) return 'O';
            if (pos==3) return 'G'; if (pos==4) return '2'; return 0;
        default: return 0;
    }
}

__DEVICE__ int tf_name_len(int tf) {
    switch (tf) {
        case 0: return 5;  case 1: return 5;  case 2: return 10;
        case 3: return 5;  case 4: return 7;  case 5: return 2;
        case 6: return 6;  case 7: return 5;  default: return 0;
    }
}

__DEVICE__ float3 render_chart(int p_Width, int p_Height, int p_X, int p_Y, int tf) {
    int px = p_X;
    int py = p_Y;
    float x_norm = (float)px / (float)p_Width;
    float y_norm = (float)py / (float)p_Height;

    // Upper half: 17 stop columns (-8 to +8)
    if (y_norm < 0.5f) {
        int num_cols = 17;
        int col = (int)(x_norm * (float)num_cols);
        if (col >= num_cols) col = num_cols - 1;
        int stop = col - 8;
        float lin = 0.18f * _powf(2.0f, (float)stop);
        float val = encode(lin, tf);
        val = _fmaxf(0.0f, _fminf(val, 1.0f));
        return make_float3(val, val, val);
    }

    // Lower half: gradient + text overlay
    float val = x_norm;
    float mid_grey = encode(0.18f, tf);

    int cell = p_Height / 100;
    if (cell < 2) cell = 2;

    int mg_d1 = (int)(mid_grey * 10.0f) % 10;
    int mg_d2 = (int)(mid_grey * 100.0f) % 10;
    int mg_d3 = (int)(mid_grey * 1000.0f + 0.5f) % 10;

    int char_w = 6 * cell;
    int char_h = 7 * cell;
    int line_gap = 3 * cell;
    int pad = 2 * cell;

    int name_len = tf_name_len(tf);
    int line2_len = 16;

    int lower_top = p_Height / 2;
    int lower_h = p_Height - lower_top;
    int total_text_h = char_h * 2 + line_gap;
    int line1_top = lower_top + (lower_h - total_text_h) / 2;
    int line2_top = line1_top + char_h + line_gap;

    int line1_left = (p_Width - name_len * char_w + cell) / 2;
    int line2_left = (p_Width - line2_len * char_w + cell) / 2;

    int box_l = (line1_left < line2_left ? line1_left : line2_left) - pad;
    int box_r = line1_left + name_len * char_w;
    int box_r2 = line2_left + line2_len * char_w;
    if (box_r2 > box_r) box_r = box_r2;
    box_r += pad;
    int box_t = line1_top - pad;
    int box_b = line2_top + char_h + pad;

    int in_box = (px >= box_l && px < box_r && py >= box_t && py < box_b) ? 1 : 0;
    int text_hit = 0;

    if (py >= line1_top && py < line1_top + char_h) {
        int lx = px - line1_left;
        if (lx >= 0 && lx < name_len * char_w) {
            int ci = lx / char_w;
            int cx = (lx % char_w) / cell;
            int cy = (py - line1_top) / cell;
            if (cx < 5) {
                int ch = tf_name_char(tf, ci);
                if (ch > 0) {
                    int mask = glyph_row(ch, cy);
                    if ((mask >> (4 - cx)) & 1) text_hit = 1;
                }
            }
        }
    }

    if (text_hit == 0 && py >= line2_top && py < line2_top + char_h) {
        int lx = px - line2_left;
        if (lx >= 0 && lx < line2_len * char_w) {
            int ci = lx / char_w;
            int cx = (lx % char_w) / cell;
            int cy = (py - line2_top) / cell;
            if (cx < 5) {
                int ch = 0;
                switch (ci) {
                    case 0: ch='1'; break; case 1: ch='8'; break;
                    case 2: ch='%'; break; case 3: ch=' '; break;
                    case 4: ch='G'; break; case 5: ch='R'; break;
                    case 6: ch='A'; break; case 7: ch='Y'; break;
                    case 8: ch=' '; break; case 9: ch='='; break;
                    case 10: ch=' '; break; case 11: ch='0'; break;
                    case 12: ch='.'; break;
                    case 13: ch='0'+mg_d1; break;
                    case 14: ch='0'+mg_d2; break;
                    case 15: ch='0'+mg_d3; break;
                }
                if (ch > 32) {
                    int mask = glyph_row(ch, cy);
                    if ((mask >> (4 - cx)) & 1) text_hit = 1;
                }
            }
        }
    }

    if (text_hit == 1) {
        val = mid_grey + 0.12f;
    } else if (in_box == 1) {
        val = mid_grey;
    }

    return make_float3(val, val, val);
}

// ============================================================
// Tone mapping primitives
// ============================================================

__DEVICE__ float soft_black_point(float x, float bp, float knee) {
    if (x <= 0.0f) return x;
    float corrected = x - bp;
    if (corrected >= knee) return corrected;
    return knee * _expf(corrected / knee - 1.0f);
}

__DEVICE__ float shadow_fill(float x, float gain, float mid) {
    if (x <= 0.0f) return x;
    if (x >= mid) return x;
    float t = x / mid;
    float blend = t * t * (3.0f - 2.0f * t);
    return x * (gain + (1.0f - gain) * blend);
}

__DEVICE__ float highlight_gain(float x, float gain, float mid) {
    if (x <= mid) return x;
    if (x >= 1.0f) return x;
    float t = (x - mid) / (1.0f - mid);
    float blend = t * t * (3.0f - 2.0f * t);
    return x * (1.0f + (gain - 1.0f) * blend);
}

__DEVICE__ float soft_highlight(float x, float amount, float mid) {
    if (x <= 0.0f) return x;
    float knee_amount = _fminf(amount, 1.0f);
    float strength = 1.0f + _fmaxf(amount - 1.0f, 0.0f) * 2.0f;
    float knee = 1.0f - knee_amount * (1.0f - mid);
    if (x <= knee) return x;
    float range = 1.0f - knee;
    float t = (x - knee) / range * strength;
    return knee + range * _tanhf(t) / strength;
}

__DEVICE__ float rolling_contrast(float x, float contrast, float pivot) {
    if (x <= 0.0f) return x;
    if (x >= 1.0f) return x;
    if (x <= pivot) {
        return _powf(x / pivot, contrast) * pivot;
    } else {
        float inv = 1.0f - x;
        float inv_pivot = 1.0f - pivot;
        return 1.0f - _powf(inv / inv_pivot, contrast) * inv_pivot;
    }
}

// ============================================================
// Main Transform
// ============================================================

__DEVICE__ float3 transform(int p_Width, int p_Height, int p_X, int p_Y, float p_R, float p_G, float p_B) {

    if (p_show_chart == 1) {
        float3 chart = render_chart(p_Width, p_Height, p_X, p_Y, p_tf);
        return chart;
    }

    float r = decode(p_R, p_tf);
    float g = decode(p_G, p_tf);
    float b = decode(p_B, p_tf);

    float gain = _powf(2.0f, p_exposure);
    r *= gain;
    g *= gain;
    b *= gain;

    if (p_bp != 0.0f) {
        float knee = 0.005f;
        float bp = -p_bp;
        r = soft_black_point(r, bp, knee);
        g = soft_black_point(g, bp, knee);
        b = soft_black_point(b, bp, knee);
    }

    r *= _powf(2.0f,  p_temp);
    b *= _powf(2.0f, -p_temp);
    g *= _powf(2.0f,  p_tint);

    r = encode(r, p_tf);
    g = encode(g, p_tf);
    b = encode(b, p_tf);

    if (p_contrast != 1.0f || p_pivot_offset != 0.0f) {
        float pivot = encode(0.18f, p_tf) + p_pivot_offset;
        pivot = _fmaxf(0.01f, _fminf(pivot, 0.99f));
        float con = (p_contrast != 1.0f) ? p_contrast : 1.0f;
        r = rolling_contrast(r, con, pivot);
        g = rolling_contrast(g, con, pivot);
        b = rolling_contrast(b, con, pivot);
    }

    if (p_shadows != 0.0f) {
        float mid = encode(0.18f, p_tf);
        float shd_gain = _powf(2.0f, p_shadows);
        r = shadow_fill(r, shd_gain, mid);
        g = shadow_fill(g, shd_gain, mid);
        b = shadow_fill(b, shd_gain, mid);
    }

    if (p_highlights != 0.0f) {
        float mid = encode(0.18f, p_tf);
        float hi_gain = _powf(2.0f, p_highlights);
        r = highlight_gain(r, hi_gain, mid);
        g = highlight_gain(g, hi_gain, mid);
        b = highlight_gain(b, hi_gain, mid);
    }

    if (p_high_soft > 0.0f) {
        float mid = encode(0.18f, p_tf);
        r = soft_highlight(r, p_high_soft, mid);
        g = soft_highlight(g, p_high_soft, mid);
        b = soft_highlight(b, p_high_soft, mid);
    }

    if (p_neg_sat < 1.0f && r >= 0.0f && g >= 0.0f && b >= 0.0f) {
        float luma_before = luminance(r, g, b);
        float luma = luma_before;
        r = luma + (r - luma) * p_neg_sat;
        g = luma + (g - luma) * p_neg_sat;
        b = luma + (b - luma) * p_neg_sat;

        if (p_preserve_lum == 1) {
            float luma_after = luminance(r, g, b);
            if (luma_after > 0.0f && luma_before > 0.0f) {
                float ratio = luma_before / luma_after;
                r *= ratio;
                g *= ratio;
                b *= ratio;
            }
        }
    }

    if (p_pos_sat > 0.0f && r >= 0.0f && g >= 0.0f && b >= 0.0f) {
        float luma_before = luminance(r, g, b);
        float3 hsv = rgb_to_hsv(make_float3(r, g, b));
        hsv.y *= _powf(2.0f, p_pos_sat);
        hsv.y = _fminf(hsv.y, 1.0f);
        float3 rgb_sat = hsv_to_rgb(hsv);
        r = rgb_sat.x;
        g = rgb_sat.y;
        b = rgb_sat.z;

        if (p_preserve_lum == 1) {
            float luma_after = luminance(r, g, b);
            if (luma_after > 0.0f && luma_before > 0.0f) {
                float ratio = luma_before / luma_after;
                r *= ratio;
                g *= ratio;
                b *= ratio;
            }
        }
    }

    return make_float3(r, g, b);
}

}

namespace PrimeraHue {
// PrimeraHue v0.6.0
// https://github.com/geoffsmithBK/primera-suite

// ============================================================
// UI Parameters
// ============================================================

float p_red_hue = 0.0;
float p_red_den = 0.0;
float p_yel_hue = 0.0;
float p_yel_den = 0.0;
float p_grn_hue = 0.0;
float p_grn_den = 0.0;
float p_cyn_hue = 0.0;
float p_cyn_den = 0.0;
float p_blu_hue = 0.0;
float p_blu_den = 0.0;
float p_mag_hue = 0.0;
float p_mag_den = 0.0;
int p_preserve_lum = 0;
int p_soft_squeeze = 1;
int p_hard_squeeze = 0;
float p_cinecolor = 0.0;
int p_protect_skin = 0;

// ============================================================
// Luminance
// ============================================================

__DEVICE__ float luminance(float r, float g, float b) {
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

__DEVICE__ float luminance_dwg(float3 rgb) {
    return 0.2627f * rgb.x + 0.6780f * rgb.y + 0.0593f * rgb.z;
}

// ============================================================
// HSV conversion
// ============================================================

__DEVICE__ float3 rgb_to_hsv(float3 rgb) {
    float r = rgb.x, g = rgb.y, b = rgb.z;
    float cmax = _fmaxf(r, _fmaxf(g, b));
    float cmin = _fminf(r, _fminf(g, b));
    float delta = cmax - cmin;

    float h = 0.0f;
    if (delta > 0.0f) {
        if (cmax == r)      h = (g - b) / delta;
        else if (cmax == g) h = (b - r) / delta + 2.0f;
        else                h = (r - g) / delta + 4.0f;
        if (h < 0.0f) h += 6.0f;
        h /= 6.0f;
    }

    float s = (cmax > 0.0f) ? delta / cmax : 0.0f;
    return make_float3(h, s, cmax);
}

__DEVICE__ float3 hsv_to_rgb(float3 hsv) {
    float h = hsv.x * 6.0f;
    float s = hsv.y;
    float v = hsv.z;

    float c = v * s;
    float x = c * (1.0f - _fabs(h - 2.0f * _floorf(h * 0.5f) - 1.0f));
    float m = v - c;

    float r, g, b;
    if      (h < 1.0f) { r = c; g = x; b = 0.0f; }
    else if (h < 2.0f) { r = x; g = c; b = 0.0f; }
    else if (h < 3.0f) { r = 0.0f; g = c; b = x; }
    else if (h < 4.0f) { r = 0.0f; g = x; b = c; }
    else if (h < 5.0f) { r = x; g = 0.0f; b = c; }
    else               { r = c; g = 0.0f; b = x; }

    return make_float3(r + m, g + m, b + m);
}

// ============================================================
// Skintone protection
// Requires: rgb_to_hsv()
// ============================================================

#define SKIN_HUE_CENTER (28.0f / 360.0f)
#define SKIN_HUE_WIDTH  (28.0f / 360.0f)

__DEVICE__ float hue_distance(float h1, float h2) {
    float d = _fabs(h1 - h2);
    return _fminf(d, 1.0f - d);
}

__DEVICE__ float smoothstep(float edge0, float edge1, float x) {
    float t = _clampf((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

__DEVICE__ float skin_mask_range(float r, float g, float b, float range) {
    float3 hsv = rgb_to_hsv(make_float3(r, g, b));
    float h = hsv.x;
    float s = hsv.y;

    if (s < 1e-6f || hsv.z < 1e-6f) return 0.0f;

    float width = SKIN_HUE_WIDTH * range;
    float dist = hue_distance(h, SKIN_HUE_CENTER);
    float hue_gate = 1.0f - smoothstep(0.0f, width, dist);
    float sat_gate = smoothstep(0.08f, 0.35f, s);

    return hue_gate * sat_gate;
}

__DEVICE__ float skin_mask(float r, float g, float b) {
    return skin_mask_range(r, g, b, 1.0f);
}

// Detection uses rg-chromaticity (normalized RGB ratios) rather than HSV
// saturation. Ratios are invariant to overall brightness, so the mask catches
// fair and dark skin equally well. Log-space skin clusters tightly around
// gn ≈ 0.33 with rn > bn.
__DEVICE__ float skin_mask_gated(float r, float g, float b, float range, float lo, float hi) {
    float sum = r + g + b;
    if (sum < 0.01f) return 0.0f;

    float rn = r / sum;
    float gn = g / sum;
    float bn = b / sum;

    // Skin chromaticity: gn sits in a narrow band, rn is red-leading vs bn.
    // Ramp widths are deliberately wide to match the smoothness of the
    // previous HSV-sat approach — narrow ramps make Density slider blotchy.
    float gn_gate = smoothstep(0.22f, 0.33f, gn) * (1.0f - smoothstep(0.37f, 0.48f, gn));
    float rb_gate = smoothstep(0.0f, 0.04f, rn - bn);
    float chroma_gate = gn_gate * rb_gate;

    // Hue gate preserves user-facing Range control (narrows/widens the pie slice)
    float3 hsv = rgb_to_hsv(make_float3(r, g, b));
    float h = hsv.x;
    float v = hsv.z;

    float width = SKIN_HUE_WIDTH * range;
    float dist = hue_distance(h, SKIN_HUE_CENTER);
    float hue_gate = 1.0f - smoothstep(0.0f, width, dist);

    // Value gates (user-facing Low/High Gate sliders)
    float lo_gate = (lo > 0.001f) ? smoothstep(lo * 0.5f, lo, v) : 1.0f;
    float hi_gate = (hi < 0.999f) ? 1.0f - smoothstep(hi - (1.0f - hi) * 0.5f, hi, v) : 1.0f;

    return hue_gate * chroma_gate * lo_gate * hi_gate;
}

// ============================================================
// Tetrahedral interpolation
// ============================================================

__DEVICE__ float3 tetra_corner(float3 identity, float hue, float density) {
    float angle = hue * 1.047198f;
    float mean = (identity.x + identity.y + identity.z) / 3.0f;
    float cx = identity.x - mean;
    float cy = identity.y - mean;
    float cz = identity.z - mean;

    float cos_a = _cosf(angle);
    float sin_a = _sinf(angle);
    float k = 0.57735026919f;

    float rx = cx * cos_a + k * (cz - cy) * sin_a;
    float ry = cy * cos_a + k * (cx - cz) * sin_a;
    float rz = cz * cos_a + k * (cy - cx) * sin_a;

    return make_float3(rx + mean + density, ry + mean + density, rz + mean + density);
}

__DEVICE__ float3 tetra_interp(float r, float g, float b,
                                float3 rc, float3 gc, float3 bc,
                                float3 cc, float3 mc, float3 yc) {
    float3 out;

    if (r >= g && r >= b) {
        if (g >= b) {
            out.x = r * rc.x + g * (yc.x - rc.x) + b * (1.0f - yc.x);
            out.y = r * rc.y + g * (yc.y - rc.y) + b * (1.0f - yc.y);
            out.z = r * rc.z + g * (yc.z - rc.z) + b * (1.0f - yc.z);
        } else {
            out.x = r * rc.x + b * (mc.x - rc.x) + g * (1.0f - mc.x);
            out.y = r * rc.y + b * (mc.y - rc.y) + g * (1.0f - mc.y);
            out.z = r * rc.z + b * (mc.z - rc.z) + g * (1.0f - mc.z);
        }
    } else if (g >= r && g >= b) {
        if (r >= b) {
            out.x = g * gc.x + r * (yc.x - gc.x) + b * (1.0f - yc.x);
            out.y = g * gc.y + r * (yc.y - gc.y) + b * (1.0f - yc.y);
            out.z = g * gc.z + r * (yc.z - gc.z) + b * (1.0f - yc.z);
        } else {
            out.x = g * gc.x + b * (cc.x - gc.x) + r * (1.0f - cc.x);
            out.y = g * gc.y + b * (cc.y - gc.y) + r * (1.0f - cc.y);
            out.z = g * gc.z + b * (cc.z - gc.z) + r * (1.0f - cc.z);
        }
    } else {
        if (r >= g) {
            out.x = b * bc.x + r * (mc.x - bc.x) + g * (1.0f - mc.x);
            out.y = b * bc.y + r * (mc.y - bc.y) + g * (1.0f - mc.y);
            out.z = b * bc.z + r * (mc.z - bc.z) + g * (1.0f - mc.z);
        } else {
            out.x = b * bc.x + g * (cc.x - bc.x) + r * (1.0f - cc.x);
            out.y = b * bc.y + g * (cc.y - bc.y) + r * (1.0f - cc.y);
            out.z = b * bc.z + g * (cc.z - bc.z) + r * (1.0f - cc.z);
        }
    }

    return out;
}

// ============================================================
// Soft gamut squeeze (tanh shoulder + exponential toe)
// ============================================================

__DEVICE__ float3 soft_squeeze(float3 rgb) {
    float r = rgb.x, g = rgb.y, b = rgb.z;

    const float knee = 0.9f;
    const float range = 0.1f;
    if (r > knee) { float t = (r - knee) / range; r = knee + range * _tanhf(t); }
    if (g > knee) { float t = (g - knee) / range; g = knee + range * _tanhf(t); }
    if (b > knee) { float t = (b - knee) / range; b = knee + range * _tanhf(t); }

    const float toe = 0.1f;
    if (r < toe) r = toe * _expf(r / toe - 1.0f);
    if (g < toe) g = toe * _expf(g / toe - 1.0f);
    if (b < toe) b = toe * _expf(b / toe - 1.0f);

    return make_float3(r, g, b);
}

// ============================================================
// Main Transform
// ============================================================

__DEVICE__ float3 transform(int p_Width, int p_Height, int p_X, int p_Y, float p_R, float p_G, float p_B) {

    float r = p_R;
    float g = p_G;
    float b = p_B;

    if (r < 0.0f || r > 1.0f || g < 0.0f || g > 1.0f || b < 0.0f || b > 1.0f)
        return make_float3(r, g, b);

    int hue_den_active = (p_red_hue != 0.0f || p_red_den != 0.0f || p_grn_hue != 0.0f || p_grn_den != 0.0f ||
                          p_blu_hue != 0.0f || p_blu_den != 0.0f || p_cyn_hue != 0.0f || p_cyn_den != 0.0f ||
                          p_mag_hue != 0.0f || p_mag_den != 0.0f || p_yel_hue != 0.0f || p_yel_den != 0.0f) ? 1 : 0;
    int cinecolor_active = (p_cinecolor > 0.0f) ? 1 : 0;

    if (!hue_den_active && !cinecolor_active)
        return make_float3(r, g, b);

    float luma_before = luminance(r, g, b);

    if (hue_den_active) {
        float3 rc = tetra_corner(make_float3(1.0f, 0.0f, 0.0f), p_red_hue, p_red_den);
        float3 gc = tetra_corner(make_float3(0.0f, 1.0f, 0.0f), p_grn_hue, p_grn_den);
        float3 bc = tetra_corner(make_float3(0.0f, 0.0f, 1.0f), p_blu_hue, p_blu_den);
        float3 cc = tetra_corner(make_float3(0.0f, 1.0f, 1.0f), p_cyn_hue, p_cyn_den);
        float3 mc = tetra_corner(make_float3(1.0f, 0.0f, 1.0f), p_mag_hue, p_mag_den);
        float3 yc = tetra_corner(make_float3(1.0f, 1.0f, 0.0f), p_yel_hue, p_yel_den);

        if (p_hard_squeeze == 1) {
            rc.x = _fmaxf(0.0f, _fminf(rc.x, 1.0f)); rc.y = _fmaxf(0.0f, _fminf(rc.y, 1.0f)); rc.z = _fmaxf(0.0f, _fminf(rc.z, 1.0f));
            gc.x = _fmaxf(0.0f, _fminf(gc.x, 1.0f)); gc.y = _fmaxf(0.0f, _fminf(gc.y, 1.0f)); gc.z = _fmaxf(0.0f, _fminf(gc.z, 1.0f));
            bc.x = _fmaxf(0.0f, _fminf(bc.x, 1.0f)); bc.y = _fmaxf(0.0f, _fminf(bc.y, 1.0f)); bc.z = _fmaxf(0.0f, _fminf(bc.z, 1.0f));
            cc.x = _fmaxf(0.0f, _fminf(cc.x, 1.0f)); cc.y = _fmaxf(0.0f, _fminf(cc.y, 1.0f)); cc.z = _fmaxf(0.0f, _fminf(cc.z, 1.0f));
            mc.x = _fmaxf(0.0f, _fminf(mc.x, 1.0f)); mc.y = _fmaxf(0.0f, _fminf(mc.y, 1.0f)); mc.z = _fmaxf(0.0f, _fminf(mc.z, 1.0f));
            yc.x = _fmaxf(0.0f, _fminf(yc.x, 1.0f)); yc.y = _fmaxf(0.0f, _fminf(yc.y, 1.0f)); yc.z = _fmaxf(0.0f, _fminf(yc.z, 1.0f));
        }

        float3 result = tetra_interp(r, g, b, rc, gc, bc, cc, mc, yc);
        r = result.x;
        g = result.y;
        b = result.z;
    }

    if (p_preserve_lum == 1) {
        float luma_after = luminance(r, g, b);
        if (luma_after > 0.0f && luma_before > 0.0f) {
            float ratio = luma_before / luma_after;
            r *= ratio;
            g *= ratio;
            b *= ratio;
        }
    }

    if (cinecolor_active) {
        float cc_t = p_cinecolor;
        float3 cc_rc = make_float3(1.0f, 0.0f, 0.0f);
        float3 cc_gc = make_float3(0.0f, 1.0f, cc_t);
        float3 cc_bc = make_float3(0.0f, 0.0f, 1.0f - cc_t);
        float3 cc_cc = make_float3(0.0f, 1.0f, 1.0f);
        float3 cc_mc = make_float3(1.0f, 0.0f, 1.0f - cc_t);
        float3 cc_yc = make_float3(1.0f, 1.0f, cc_t);

        float cr = _fmaxf(0.0f, _fminf(r, 1.0f));
        float cg = _fmaxf(0.0f, _fminf(g, 1.0f));
        float cb = _fmaxf(0.0f, _fminf(b, 1.0f));

        float3 cc_result = tetra_interp(cr, cg, cb, cc_rc, cc_gc, cc_bc, cc_cc, cc_mc, cc_yc);

        if (p_protect_skin == 1) {
            float prot = skin_mask(cr, cg, cb);
            cc_result.z = cc_result.z * (1.0f - prot) + cb * prot;
        }

        r = cc_result.x;
        g = cc_result.y;
        b = cc_result.z;
    }

    if (p_soft_squeeze == 1) {
        float3 squeezed = soft_squeeze(make_float3(r, g, b));
        r = squeezed.x;
        g = squeezed.y;
        b = squeezed.z;
    }

    return make_float3(r, g, b);
}

}
#undef SKIN_HUE_CENTER
#undef SKIN_HUE_WIDTH
namespace PrimeraSplit {
// PrimeraSplit v0.6.0
// https://github.com/geoffsmithBK/primera-suite

// ============================================================
// UI Parameters
// ===============================================================

int p_tf = 0;
float pivot = 0.0;
float transition = 0.2;

float shadow_red = 0.0;
float shadow_green = 0.0;
float shadow_blue = 0.0;
float shadow_cyan = 0.0;
float shadow_magenta = 0.0;
float shadow_yellow = 0.0;

float highlight_red = 0.0;
float highlight_green = 0.0;
float highlight_blue = 0.0;
float highlight_cyan = 0.0;
float highlight_magenta = 0.0;
float highlight_yellow = 0.0;

float preserve_luminance = 0.8;
int opt_showcurve = 0;
int visualize = 0;
int opt_showramp = 0;
float ramp_pos = 0.0;
int p_show_chart = 0;
// ============================================================
// Luminance
// ============================================================

__DEVICE__ float luminance(float r, float g, float b) {
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

__DEVICE__ float luminance_dwg(float3 rgb) {
    return 0.2627f * rgb.x + 0.6780f * rgb.y + 0.0593f * rgb.z;
}
// ============================================================
// Transfer functions: linear -> log
// ============================================================

__DEVICE__ float lin_to_logc3(float x) {
    const float a = 5.555556f;
    const float b = 0.052272f;
    const float c = 0.247190f;
    const float d = 0.385537f;
    const float e = 5.367655f;
    const float f = 0.092809f;
    const float cut = 0.010591f;
    return (x > cut) ? c * _log10f(a * x + b) + d : e * x + f;
}

__DEVICE__ float lin_to_logc4(float x) {
    const float a = (262144.0f - 16.0f) / 117.45f;
    const float b = (1023.0f - 95.0f) / 1023.0f;
    const float c = 95.0f / 1023.0f;
    const float s = (7.0f * 0.6931471805599453f * _powf(2.0f, 7.0f - 14.0f * c / b)) / (a * b);
    const float t = (_powf(2.0f, 14.0f * (-c / b) + 6.0f) - 64.0f) / a;
    return (x >= t) ? (_log2f(a * x + 64.0f) - 6.0f) / 14.0f * b + c : (x - t) / s;
}

__DEVICE__ float lin_to_redlog3g10(float x) {
    const float a = 0.224282f;
    const float b = 155.975327f;
    const float c = 0.01f;
    const float g = 15.1927f;
    float xc = x + c;
    return (xc < 0.0f) ? xc * g : a * _log10f(xc * b + 1.0f);
}

__DEVICE__ float lin_to_slog3(float x) {
    const float cut = 0.01125f;
    if (x >= cut)
        return (420.0f + _log10f((x + 0.01f) / 0.19f) * 261.5f) / 1023.0f;
    else
        return (x * (171.2102946929f - 95.0f) / 0.01125f + 95.0f) / 1023.0f;
}

__DEVICE__ float lin_to_acescct(float x) {
    const float cut = 0.0078125f;
    const float slope = 10.5402377416545f;
    const float offset = 0.0729055341958355f;
    return (x > cut) ? (_log2f(x) + 9.72f) / 17.52f : slope * x + offset;
}

__DEVICE__ float lin_to_di(float x) {
    const float A = 0.0075f;
    const float B = 7.0f;
    const float C = 0.07329248f;
    const float M = 10.44426855f;
    const float LIN_CUT = 0.00262409f;
    return (x <= LIN_CUT) ? x * M : C * (_log2f(x + A) + B);
}

__DEVICE__ float lin_to_cineon(float x) {
    const float offset = 0.0108f;
    return (_log10f(x * (1.0f - offset) + offset) * 300.0f + 685.0f) / 1023.0f;
}

__DEVICE__ float lin_to_flog2(float x) {
    const float a = 5.555556f;
    const float b = 0.064829f;
    const float c = 0.245281f;
    const float d = 0.384316f;
    const float e = 8.799461f;
    const float f = 0.092864f;
    const float cut1 = 0.000889f;
    return (x >= cut1) ? c * _log10f(a * x + b) + d : e * x + f;
}

__DEVICE__ float lin_to_vlog(float x) {
    const float b = 0.00873f;
    const float c = 0.241514f;
    const float d = 0.598206f;
    const float cut1 = 0.01f;
    return (x < cut1) ? 5.6f * x + 0.125f : c * _log10f(x + b) + d;
}

__DEVICE__ float encode(float x, int tf) {
    switch (tf) {
        case 0: return lin_to_logc3(x);
        case 1: return lin_to_logc4(x);
        case 2: return lin_to_redlog3g10(x);
        case 3: return lin_to_slog3(x);
        case 4: return lin_to_acescct(x);
        case 5: return lin_to_di(x);
        case 6: return lin_to_cineon(x);
        case 7: return lin_to_flog2(x);
        case 8: return lin_to_vlog(x);
        default: return x;
    }
}
// ============================================================
// 5x7 bitmap glyph table
// Each row returns 5 bits (LSB = rightmost column).
// Used by chart overlays and the PrimeraSkin Show Mask legend.
// ============================================================

__DEVICE__ int glyph_row(int ch, int row) {
    switch (ch) {
        case '0': return (row==0||row==6)?31:27;
        case '1': return row==1?14:row==6?31:6;
        case '2': return (row==0||row==3||row==6)?31:row<3?3:24;
        case '3': return (row==0||row==3||row==6)?31:3;
        case '4': return row<=2?27:row==3?31:3;
        case '5': return (row==0||row==3||row==6)?31:row<3?24:3;
        case '6': return (row==0||row==3||row==6)?31:row<3?24:27;
        case '7': return row==0?31:row<=2?3:6;
        case '8': return (row==0||row==3||row==6)?31:27;
        case '9': return (row==0||row==3||row==6)?31:row<3?27:3;
        case 'A': return (row==0||row==3)?31:27;
        case 'C': return (row==0||row==6)?31:24;
        case 'D': return (row==0||row==6)?30:27;
        case 'E': return (row==0||row==3||row==6)?31:24;
        case 'F': return (row==0||row==3)?31:24;
        case 'G': return (row==0||row==6)?31:row<=2?24:27;
        case 'I': return (row==0||row==6)?31:6;
        case 'K': return row==0?17:row==1?18:row==2?20:row==3?24:row==4?20:row==5?18:17;
        case 'L': return row==6?31:24;
        case 'M': return row==0?17:row<=2?27:row==3?21:17;
        case 'N': return (row==0||row==6)?17:row<=2?25:row==3?21:19;
        case 'O': return (row==0||row==6)?31:27;
        case 'R': return (row==0||row==3)?31:row<=2?27:row==4?26:25;
        case 'S': return (row==0||row==3||row==6)?31:row<3?24:3;
        case 'T': return row==0?31:6;
        case 'Y': return row<=1?27:row==2?14:6;
        case '%': return row==0?25:row==1?26:row==2?2:row==3?4:row==4?8:row==5?19:3;
        case '=': return (row==2||row==4)?31:0;
        case '.': return row>=5?12:0;
        default:  return 0;
    }
}

// ============================================================
// Stop chart (renderer)
// Requires: encode(), glyph_row()
// ============================================================

__DEVICE__ int tf_name_char(int tf, int pos) {
    switch (tf) {
        case 0:
            if (pos==0) return 'L'; if (pos==1) return 'O'; if (pos==2) return 'G';
            if (pos==3) return 'C'; if (pos==4) return '3'; return 0;
        case 1:
            if (pos==0) return 'L'; if (pos==1) return 'O'; if (pos==2) return 'G';
            if (pos==3) return 'C'; if (pos==4) return '4'; return 0;
        case 2:
            if (pos==0) return 'R'; if (pos==1) return 'E'; if (pos==2) return 'D';
            if (pos==3) return 'L'; if (pos==4) return 'O'; if (pos==5) return 'G';
            if (pos==6) return '3'; if (pos==7) return 'G'; if (pos==8) return '1';
            if (pos==9) return '0'; return 0;
        case 3:
            if (pos==0) return 'S'; if (pos==1) return 'L'; if (pos==2) return 'O';
            if (pos==3) return 'G'; if (pos==4) return '3'; return 0;
        case 4:
            if (pos==0) return 'A'; if (pos==1) return 'C'; if (pos==2) return 'E';
            if (pos==3) return 'S'; if (pos==4) return 'C'; if (pos==5) return 'C';
            if (pos==6) return 'T'; return 0;
        case 5:
            if (pos==0) return 'D'; if (pos==1) return 'I'; return 0;
        case 6:
            if (pos==0) return 'C'; if (pos==1) return 'I'; if (pos==2) return 'N';
            if (pos==3) return 'E'; if (pos==4) return 'O'; if (pos==5) return 'N';
            return 0;
        case 7:
            if (pos==0) return 'F'; if (pos==1) return 'L'; if (pos==2) return 'O';
            if (pos==3) return 'G'; if (pos==4) return '2'; return 0;
        default: return 0;
    }
}

__DEVICE__ int tf_name_len(int tf) {
    switch (tf) {
        case 0: return 5;  case 1: return 5;  case 2: return 10;
        case 3: return 5;  case 4: return 7;  case 5: return 2;
        case 6: return 6;  case 7: return 5;  default: return 0;
    }
}

__DEVICE__ float3 render_chart(int p_Width, int p_Height, int p_X, int p_Y, int tf) {
    int px = p_X;
    int py = p_Y;
    float x_norm = (float)px / (float)p_Width;
    float y_norm = (float)py / (float)p_Height;

    // Upper half: 17 stop columns (-8 to +8)
    if (y_norm < 0.5f) {
        int num_cols = 17;
        int col = (int)(x_norm * (float)num_cols);
        if (col >= num_cols) col = num_cols - 1;
        int stop = col - 8;
        float lin = 0.18f * _powf(2.0f, (float)stop);
        float val = encode(lin, tf);
        val = _fmaxf(0.0f, _fminf(val, 1.0f));
        return make_float3(val, val, val);
    }

    // Lower half: gradient + text overlay
    float val = x_norm;
    float mid_grey = encode(0.18f, tf);

    int cell = p_Height / 100;
    if (cell < 2) cell = 2;

    int mg_d1 = (int)(mid_grey * 10.0f) % 10;
    int mg_d2 = (int)(mid_grey * 100.0f) % 10;
    int mg_d3 = (int)(mid_grey * 1000.0f + 0.5f) % 10;

    int char_w = 6 * cell;
    int char_h = 7 * cell;
    int line_gap = 3 * cell;
    int pad = 2 * cell;

    int name_len = tf_name_len(tf);
    int line2_len = 16;

    int lower_top = p_Height / 2;
    int lower_h = p_Height - lower_top;
    int total_text_h = char_h * 2 + line_gap;
    int line1_top = lower_top + (lower_h - total_text_h) / 2;
    int line2_top = line1_top + char_h + line_gap;

    int line1_left = (p_Width - name_len * char_w + cell) / 2;
    int line2_left = (p_Width - line2_len * char_w + cell) / 2;

    int box_l = (line1_left < line2_left ? line1_left : line2_left) - pad;
    int box_r = line1_left + name_len * char_w;
    int box_r2 = line2_left + line2_len * char_w;
    if (box_r2 > box_r) box_r = box_r2;
    box_r += pad;
    int box_t = line1_top - pad;
    int box_b = line2_top + char_h + pad;

    int in_box = (px >= box_l && px < box_r && py >= box_t && py < box_b) ? 1 : 0;
    int text_hit = 0;

    if (py >= line1_top && py < line1_top + char_h) {
        int lx = px - line1_left;
        if (lx >= 0 && lx < name_len * char_w) {
            int ci = lx / char_w;
            int cx = (lx % char_w) / cell;
            int cy = (py - line1_top) / cell;
            if (cx < 5) {
                int ch = tf_name_char(tf, ci);
                if (ch > 0) {
                    int mask = glyph_row(ch, cy);
                    if ((mask >> (4 - cx)) & 1) text_hit = 1;
                }
            }
        }
    }

    if (text_hit == 0 && py >= line2_top && py < line2_top + char_h) {
        int lx = px - line2_left;
        if (lx >= 0 && lx < line2_len * char_w) {
            int ci = lx / char_w;
            int cx = (lx % char_w) / cell;
            int cy = (py - line2_top) / cell;
            if (cx < 5) {
                int ch = 0;
                switch (ci) {
                    case 0: ch='1'; break; case 1: ch='8'; break;
                    case 2: ch='%'; break; case 3: ch=' '; break;
                    case 4: ch='G'; break; case 5: ch='R'; break;
                    case 6: ch='A'; break; case 7: ch='Y'; break;
                    case 8: ch=' '; break; case 9: ch='='; break;
                    case 10: ch=' '; break; case 11: ch='0'; break;
                    case 12: ch='.'; break;
                    case 13: ch='0'+mg_d1; break;
                    case 14: ch='0'+mg_d2; break;
                    case 15: ch='0'+mg_d3; break;
                }
                if (ch > 32) {
                    int mask = glyph_row(ch, cy);
                    if ((mask >> (4 - cx)) & 1) text_hit = 1;
                }
            }
        }
    }

    if (text_hit == 1) {
        val = mid_grey + 0.12f;
    } else if (in_box == 1) {
        val = mid_grey;
    }

    return make_float3(val, val, val);
}

// ============================================================
// Subtractive split toning
// ============================================================

__DEVICE__ float smooth_step(float edge0, float edge1, float x) {
    float t = _clampf((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

__DEVICE__ float calculate_subtractive_impact(float3 rgb_adj, float3 cmy_adj) {
    float total_reduction = 0.0f;

    total_reduction += _fmaxf(0.0f, rgb_adj.x) * 0.5f;
    total_reduction += _fmaxf(0.0f, rgb_adj.y) * 0.5f;
    total_reduction += _fmaxf(0.0f, rgb_adj.z) * 0.5f;

    total_reduction += _fmaxf(0.0f, -rgb_adj.x) * 0.33f;
    total_reduction += _fmaxf(0.0f, -rgb_adj.y) * 0.33f;
    total_reduction += _fmaxf(0.0f, -rgb_adj.z) * 0.33f;

    total_reduction += _fmaxf(0.0f, cmy_adj.x) * 0.33f;
    total_reduction += _fmaxf(0.0f, cmy_adj.y) * 0.33f;
    total_reduction += _fmaxf(0.0f, cmy_adj.z) * 0.33f;

    total_reduction += _fmaxf(0.0f, -cmy_adj.x) * 0.5f;
    total_reduction += _fmaxf(0.0f, -cmy_adj.y) * 0.5f;
    total_reduction += _fmaxf(0.0f, -cmy_adj.z) * 0.5f;

    return total_reduction;
}

__DEVICE__ float calculate_luminance_compensation(float subtractive_impact) {
    if (subtractive_impact <= 0.0f) return 1.0f;
    float compensation = 1.0f + (subtractive_impact * 2.0f);
    return _clampf(compensation, 1.0f, 2.0f);
}

__DEVICE__ float3 apply_subtractive_adjustment(float3 rgb, float3 rgb_adjustment, float3 cmy_adjustment) {
    float3 result = rgb;

    if (rgb_adjustment.x > 0.0f) {
        result.y *= (1.0f - rgb_adjustment.x);
        result.z *= (1.0f - rgb_adjustment.x);
    } else if (rgb_adjustment.x < 0.0f) {
        result.x *= (1.0f + rgb_adjustment.x);
    }

    if (rgb_adjustment.y > 0.0f) {
        result.x *= (1.0f - rgb_adjustment.y);
        result.z *= (1.0f - rgb_adjustment.y);
    } else if (rgb_adjustment.y < 0.0f) {
        result.y *= (1.0f + rgb_adjustment.y);
    }

    if (rgb_adjustment.z > 0.0f) {
        result.x *= (1.0f - rgb_adjustment.z);
        result.y *= (1.0f - rgb_adjustment.z);
    } else if (rgb_adjustment.z < 0.0f) {
        result.z *= (1.0f + rgb_adjustment.z);
    }

    if (cmy_adjustment.x > 0.0f) {
        result.x *= (1.0f - cmy_adjustment.x);
    } else if (cmy_adjustment.x < 0.0f) {
        result.y *= (1.0f + cmy_adjustment.x);
        result.z *= (1.0f + cmy_adjustment.x);
    }

    if (cmy_adjustment.y > 0.0f) {
        result.y *= (1.0f - cmy_adjustment.y);
    } else if (cmy_adjustment.y < 0.0f) {
        result.x *= (1.0f + cmy_adjustment.y);
        result.z *= (1.0f + cmy_adjustment.y);
    }

    if (cmy_adjustment.z > 0.0f) {
        result.z *= (1.0f - cmy_adjustment.z);
    } else if (cmy_adjustment.z < 0.0f) {
        result.x *= (1.0f + cmy_adjustment.z);
        result.y *= (1.0f + cmy_adjustment.z);
    }

    return result;
}

__DEVICE__ float3 draw_curve_overlay(float3 rgb, int p_X, int p_Y, int p_Width, int p_Height, float pivot_val, float transition_val) {
    float x_norm = (float)p_X / (float)p_Width;
    float y_norm = 1.0f - (float)p_Y / (float)p_Height;

    float shadow_edge = pivot_val - transition_val * 0.5f;
    float highlight_edge = pivot_val + transition_val * 0.5f;

    float shadow_weight = 1.0f - smooth_step(shadow_edge, highlight_edge, x_norm);
    float highlight_weight = smooth_step(shadow_edge, highlight_edge, x_norm);

    float curve_thickness = 0.003f;

    float shadow_curve_y = 0.7f + shadow_weight * 0.2f;
    if (_fabs(y_norm - shadow_curve_y) < curve_thickness) {
        rgb = _mix(rgb, make_float3(1.0f, 0.3f, 0.3f), 0.7f);
    }

    float highlight_curve_y = 0.3f + highlight_weight * 0.2f;
    if (_fabs(y_norm - highlight_curve_y) < curve_thickness) {
        rgb = _mix(rgb, make_float3(0.3f, 0.5f, 1.0f), 0.7f);
    }

    float pivot_x = pivot_val;
    if (_fabs(x_norm - pivot_x) < curve_thickness) {
        rgb = _mix(rgb, make_float3(1.0f, 1.0f, 0.3f), 0.5f);
    }

    return rgb;
}

// ============================================================
// Main Transform
// ============================================================

__DEVICE__ float3 transform(int p_Width, int p_Height, int p_X, int p_Y, float p_R, float p_G, float p_B) {

    float effective_pivot = encode(0.18f, p_tf) + pivot;

    if (p_show_chart == 1) {
        float3 chart = render_chart(p_Width, p_Height, p_X, p_Y, p_tf);
        return chart;
    }

    float3 rgb = make_float3(p_R, p_G, p_B);

    float width = (float)p_Width;
    float height = (float)p_Height;
    float X = (float)p_X;
    float Y = height - (float)p_Y;
    float3 ramp = make_float3(X / width, X / width, X / width);

    float luma = luminance_dwg(rgb);

    float shadow_edge = effective_pivot - transition * 0.5f;
    float highlight_edge = effective_pivot + transition * 0.5f;

    float shadow_weight = 1.0f - smooth_step(shadow_edge, highlight_edge, luma);
    float highlight_weight = smooth_step(shadow_edge, highlight_edge, luma);

    float3 shadow_rgb_adj = make_float3(shadow_red, shadow_green, shadow_blue);
    float3 shadow_cmy_adj = make_float3(shadow_cyan, shadow_magenta, shadow_yellow);
    float3 highlight_rgb_adj = make_float3(highlight_red, highlight_green, highlight_blue);
    float3 highlight_cmy_adj = make_float3(highlight_cyan, highlight_magenta, highlight_yellow);

    float3 final_shadow_rgb = shadow_rgb_adj * shadow_weight;
    float3 final_shadow_cmy = shadow_cmy_adj * shadow_weight;
    float3 final_highlight_rgb = highlight_rgb_adj * highlight_weight;
    float3 final_highlight_cmy = highlight_cmy_adj * highlight_weight;

    float3 combined_rgb_adj = final_shadow_rgb + final_highlight_rgb;
    float3 combined_cmy_adj = final_shadow_cmy + final_highlight_cmy;

    rgb = apply_subtractive_adjustment(rgb, combined_rgb_adj, combined_cmy_adj);

    if (preserve_luminance > 0.0f) {
        float subtractive_impact = calculate_subtractive_impact(combined_rgb_adj, combined_cmy_adj);
        float compensation = calculate_luminance_compensation(subtractive_impact);
        rgb *= 1.0f + (compensation - 1.0f) * preserve_luminance;
    }

    float ramp_luma = X / width;
    float ramp_shadow_weight = 1.0f - smooth_step(shadow_edge, highlight_edge, ramp_luma);
    float ramp_highlight_weight = smooth_step(shadow_edge, highlight_edge, ramp_luma);

    float3 ramp_shadow_rgb = shadow_rgb_adj * ramp_shadow_weight;
    float3 ramp_shadow_cmy = shadow_cmy_adj * ramp_shadow_weight;
    float3 ramp_highlight_rgb = highlight_rgb_adj * ramp_highlight_weight;
    float3 ramp_highlight_cmy = highlight_cmy_adj * ramp_highlight_weight;

    float3 ramp_combined_rgb_adj = ramp_shadow_rgb + ramp_highlight_rgb;
    float3 ramp_combined_cmy_adj = ramp_shadow_cmy + ramp_highlight_cmy;

    ramp = apply_subtractive_adjustment(ramp, ramp_combined_rgb_adj, ramp_combined_cmy_adj);

    if (preserve_luminance > 0.0f) {
        float ramp_subtractive_impact = calculate_subtractive_impact(ramp_combined_rgb_adj, ramp_combined_cmy_adj);
        float ramp_compensation = calculate_luminance_compensation(ramp_subtractive_impact);
        ramp *= 1.0f + (ramp_compensation - 1.0f) * preserve_luminance;
    }

    if (visualize) {
        rgb = draw_curve_overlay(rgb, p_X, p_Y, p_Width, p_Height, effective_pivot, transition);
    }

    if (opt_showcurve == 1) {
        float overlayR = ramp.x >= (Y - 5.0f) / height && ramp.x <= (Y + 5.0f) / height ? 1.0f : 0.0f;
        float overlayG = ramp.y >= (Y - 5.0f) / height && ramp.y <= (Y + 5.0f) / height ? 1.0f : 0.0f;
        float overlayB = ramp.z >= (Y - 5.0f) / height && ramp.z <= (Y + 5.0f) / height ? 1.0f : 0.0f;

        rgb.x = overlayR == 0.0f ? rgb.x : overlayR;
        rgb.y = overlayG == 0.0f ? rgb.y : overlayG;
        rgb.z = overlayB == 0.0f ? rgb.z : overlayB;
    }

    if (opt_showramp == 1) {
        float y_norm = 1.0f - (float)p_Y / (float)p_Height;
        if (y_norm >= ramp_pos && y_norm <= ramp_pos + 0.15f) {
            rgb.x = ramp.x;
            rgb.y = ramp.y;
            rgb.z = ramp.z;
        }
    }

    return rgb;
}

}

namespace PrimeraSkin {
// PrimeraSkin v0.6.0
// https://github.com/geoffsmithBK/primera-suite

// ============================================================
// UI Parameters
// ============================================================

float p_hue = 0.0;
float p_saturation = 0.0;
float p_density = 0.0;
float p_range = 1.0;
float p_soften = 0.0;
float p_evenness = 0.0;
float p_separation = 0.0;
float p_lo_gate = 0.0;
float p_hi_gate = 1.0;
int p_show_mask = 0;
int p_legend = 0;

// ============================================================
// HSV conversion
// ============================================================

__DEVICE__ float3 rgb_to_hsv(float3 rgb) {
    float r = rgb.x, g = rgb.y, b = rgb.z;
    float cmax = _fmaxf(r, _fmaxf(g, b));
    float cmin = _fminf(r, _fminf(g, b));
    float delta = cmax - cmin;

    float h = 0.0f;
    if (delta > 0.0f) {
        if (cmax == r)      h = (g - b) / delta;
        else if (cmax == g) h = (b - r) / delta + 2.0f;
        else                h = (r - g) / delta + 4.0f;
        if (h < 0.0f) h += 6.0f;
        h /= 6.0f;
    }

    float s = (cmax > 0.0f) ? delta / cmax : 0.0f;
    return make_float3(h, s, cmax);
}

__DEVICE__ float3 hsv_to_rgb(float3 hsv) {
    float h = hsv.x * 6.0f;
    float s = hsv.y;
    float v = hsv.z;

    float c = v * s;
    float x = c * (1.0f - _fabs(h - 2.0f * _floorf(h * 0.5f) - 1.0f));
    float m = v - c;

    float r, g, b;
    if      (h < 1.0f) { r = c; g = x; b = 0.0f; }
    else if (h < 2.0f) { r = x; g = c; b = 0.0f; }
    else if (h < 3.0f) { r = 0.0f; g = c; b = x; }
    else if (h < 4.0f) { r = 0.0f; g = x; b = c; }
    else if (h < 5.0f) { r = x; g = 0.0f; b = c; }
    else               { r = c; g = 0.0f; b = x; }

    return make_float3(r + m, g + m, b + m);
}

// ============================================================
// Skintone protection
// Requires: rgb_to_hsv()
// ============================================================

#define SKIN_HUE_CENTER (28.0f / 360.0f)
#define SKIN_HUE_WIDTH  (28.0f / 360.0f)

__DEVICE__ float hue_distance(float h1, float h2) {
    float d = _fabs(h1 - h2);
    return _fminf(d, 1.0f - d);
}

__DEVICE__ float smoothstep(float edge0, float edge1, float x) {
    float t = _clampf((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

__DEVICE__ float skin_mask_range(float r, float g, float b, float range) {
    float3 hsv = rgb_to_hsv(make_float3(r, g, b));
    float h = hsv.x;
    float s = hsv.y;

    if (s < 1e-6f || hsv.z < 1e-6f) return 0.0f;

    float width = SKIN_HUE_WIDTH * range;
    float dist = hue_distance(h, SKIN_HUE_CENTER);
    float hue_gate = 1.0f - smoothstep(0.0f, width, dist);
    float sat_gate = smoothstep(0.08f, 0.35f, s);

    return hue_gate * sat_gate;
}

__DEVICE__ float skin_mask(float r, float g, float b) {
    return skin_mask_range(r, g, b, 1.0f);
}

// Detection uses rg-chromaticity (normalized RGB ratios) rather than HSV
// saturation. Ratios are invariant to overall brightness, so the mask catches
// fair and dark skin equally well. Log-space skin clusters tightly around
// gn ≈ 0.33 with rn > bn.
__DEVICE__ float skin_mask_gated(float r, float g, float b, float range, float lo, float hi) {
    float sum = r + g + b;
    if (sum < 0.01f) return 0.0f;

    float rn = r / sum;
    float gn = g / sum;
    float bn = b / sum;

    // Skin chromaticity: gn sits in a narrow band, rn is red-leading vs bn.
    // Ramp widths are deliberately wide to match the smoothness of the
    // previous HSV-sat approach — narrow ramps make Density slider blotchy.
    float gn_gate = smoothstep(0.22f, 0.33f, gn) * (1.0f - smoothstep(0.37f, 0.48f, gn));
    float rb_gate = smoothstep(0.0f, 0.04f, rn - bn);
    float chroma_gate = gn_gate * rb_gate;

    // Hue gate preserves user-facing Range control (narrows/widens the pie slice)
    float3 hsv = rgb_to_hsv(make_float3(r, g, b));
    float h = hsv.x;
    float v = hsv.z;

    float width = SKIN_HUE_WIDTH * range;
    float dist = hue_distance(h, SKIN_HUE_CENTER);
    float hue_gate = 1.0f - smoothstep(0.0f, width, dist);

    // Value gates (user-facing Low/High Gate sliders)
    float lo_gate = (lo > 0.001f) ? smoothstep(lo * 0.5f, lo, v) : 1.0f;
    float hi_gate = (hi < 0.999f) ? 1.0f - smoothstep(hi - (1.0f - hi) * 0.5f, hi, v) : 1.0f;

    return hue_gate * chroma_gate * lo_gate * hi_gate;
}

// ============================================================
// Soft gamut squeeze (tanh shoulder + exponential toe)
// ============================================================

__DEVICE__ float3 soft_squeeze(float3 rgb) {
    float r = rgb.x, g = rgb.y, b = rgb.z;

    const float knee = 0.9f;
    const float range = 0.1f;
    if (r > knee) { float t = (r - knee) / range; r = knee + range * _tanhf(t); }
    if (g > knee) { float t = (g - knee) / range; g = knee + range * _tanhf(t); }
    if (b > knee) { float t = (b - knee) / range; b = knee + range * _tanhf(t); }

    const float toe = 0.1f;
    if (r < toe) r = toe * _expf(r / toe - 1.0f);
    if (g < toe) g = toe * _expf(g / toe - 1.0f);
    if (b < toe) b = toe * _expf(b / toe - 1.0f);

    return make_float3(r, g, b);
}

// ============================================================
// 5x7 bitmap glyph table
// Each row returns 5 bits (LSB = rightmost column).
// Used by chart overlays and the PrimeraSkin Show Mask legend.
// ============================================================

__DEVICE__ int glyph_row(int ch, int row) {
    switch (ch) {
        case '0': return (row==0||row==6)?31:27;
        case '1': return row==1?14:row==6?31:6;
        case '2': return (row==0||row==3||row==6)?31:row<3?3:24;
        case '3': return (row==0||row==3||row==6)?31:3;
        case '4': return row<=2?27:row==3?31:3;
        case '5': return (row==0||row==3||row==6)?31:row<3?24:3;
        case '6': return (row==0||row==3||row==6)?31:row<3?24:27;
        case '7': return row==0?31:row<=2?3:6;
        case '8': return (row==0||row==3||row==6)?31:27;
        case '9': return (row==0||row==3||row==6)?31:row<3?27:3;
        case 'A': return (row==0||row==3)?31:27;
        case 'C': return (row==0||row==6)?31:24;
        case 'D': return (row==0||row==6)?30:27;
        case 'E': return (row==0||row==3||row==6)?31:24;
        case 'F': return (row==0||row==3)?31:24;
        case 'G': return (row==0||row==6)?31:row<=2?24:27;
        case 'I': return (row==0||row==6)?31:6;
        case 'K': return row==0?17:row==1?18:row==2?20:row==3?24:row==4?20:row==5?18:17;
        case 'L': return row==6?31:24;
        case 'M': return row==0?17:row<=2?27:row==3?21:17;
        case 'N': return (row==0||row==6)?17:row<=2?25:row==3?21:19;
        case 'O': return (row==0||row==6)?31:27;
        case 'R': return (row==0||row==3)?31:row<=2?27:row==4?26:25;
        case 'S': return (row==0||row==3||row==6)?31:row<3?24:3;
        case 'T': return row==0?31:6;
        case 'Y': return row<=1?27:row==2?14:6;
        case '%': return row==0?25:row==1?26:row==2?2:row==3?4:row==4?8:row==5?19:3;
        case '=': return (row==2||row==4)?31:0;
        case '.': return row>=5?12:0;
        default:  return 0;
    }
}

// ============================================================
// Transform
// ============================================================

#define COMPRESS_EXTRA 0.4f
#define LEGEND_STRIP_H 0.075f

// Representative skin chromaticity (R-leading, B-trailing). Used as the
// reference base for legend chip rendering so the strip reflects what the
// overlay actually produces on real skin rather than on uniform grey.
#define SKIN_BASE_R 0.55f
#define SKIN_BASE_G 0.42f
#define SKIN_BASE_B 0.25f

#define GLYPH_GREY 0.15f

// Separation (see color-contrast.MD): non-skin hues migrate toward the band
// opposite the skin vector. The target is derived, not user-facing.
#define SEP_COMPLEMENT      (SKIN_HUE_CENTER + 0.5f)   // ~208 deg (teal)
#define SEP_FIELD_GAIN      0.075f   // 27 deg max shift at the 90-deg flanks: ~30% migration cap
#define SEP_FALLBACK_SOFTEN 0.5f     // MINIMUM pooling strength for the Separation gate

// Continuous mapping from zone position (-1..+1) to chromaticity-shifted RGB.
// zone=-1: cyan/green side   zone=0: skin centre (gold)   zone=+1: magenta side
// Used by both the Show Mask overlay (per-pixel) and the Legend strip
// (gradient across x). Same gains drive both, so the strip is a closed-form
// rendering of the overlay function applied to a fixed skin base.
__DEVICE__ float3 zone_color(float zone, float3 base) {
    if (zone < -1.0f) zone = -1.0f;
    if (zone > 1.0f) zone = 1.0f;

    float gr, gg, gb;
    if (zone <= 0.0f) {
        float t = -zone;
        gr = 1.4f;
        gg = 1.3f - t * 0.8f;
        gb = 0.5f + t * 0.8f;
    } else {
        float t = zone;
        gr = 1.4f - t * 0.9f;
        gg = 1.3f;
        gb = 0.5f + t * 0.8f;
    }

    return make_float3(base.x * gr, base.y * gg, base.z * gb);
}

// ------------------------------------------------------------
// Spatial mask pooling (Soften)
// ------------------------------------------------------------
// The per-pixel chromaticity mask (skin_mask_gated) is a pure function of one
// pixel's RGB, so sensor grain dithers borderline skin pixels across the gate
// edges — leaving a stochastic "salt" of under-adjusted pixels inside solid
// skin (the Density slider's pointillist look). The fix is spatial: pool the
// mask over a sparse neighbourhood with a loose bilateral (chroma-similarity)
// weight, then SOFT-UNION the pooled value with the pixel's own mask so pooling
// only FILLS holes and never erodes confident edges.
//
// This helper lives here (not in skintone.dctlf) because that fragment is
// shared with PrimeraHue, which keeps the scalar transform signature and must
// not gain texture dependencies.

// Sparse tap pattern: centre + 3 concentric rings (8/12/16 taps) at r/3, 2r/3,
// r. Ring angles are golden-angle staggered (137.5°) so taps never line up on
// image axes (avoids axis-aligned banding). cos/sin computed per tap in fixed
// loops — fixed-bound, so acceptable in DCTL.
#define POOL_GOLDEN 2.39996323f   // golden angle in radians (~137.5°)

__DEVICE__ float pool_skin_mask(__TEXTURE__ p_TexR, __TEXTURE__ p_TexG, __TEXTURE__ p_TexB,
                                int p_Width, int p_Height, int p_X, int p_Y,
                                float r_c, float g_c, float b_c,
                                float m_own, float radius_px,
                                float range, float lo, float hi) {
    // Centre chroma reference in rg-chromaticity space.
    float sum_c = r_c + g_c + b_c;
    float gn_c, dn_c;
    int center_valid = 0;
    if (sum_c >= 0.01f) {
        gn_c = g_c / sum_c;
        dn_c = (r_c - b_c) / sum_c;   // rn - bn
        center_valid = 1;
    } else {
        gn_c = 0.0f;
        dn_c = 0.0f;
    }

    const float sigma_s = radius_px * 0.5f;          // spatial Gaussian sigma
    const float inv2ss = 1.0f / (2.0f * sigma_s * sigma_s);
    const float sigma_c = 0.06f;                     // chroma-similarity sigma
    const float inv2sc = 1.0f / (2.0f * sigma_c * sigma_c);

    // Accumulate centre tap first.
    float wsum = 1.0f;          // centre spatial gaussian = exp(0) = 1
    float msum = m_own;         // centre similarity = 1, mask = m_own

    // Ring configuration: radii fractions and tap counts.
    const int   ring_counts[3] = { 8, 12, 16 };
    const float ring_fracs[3]  = { 0.3333333f, 0.6666667f, 1.0f };
    // Per-ring starting phase, staggered by the golden angle so rings don't
    // share spoke directions.
    float phase = 0.0f;

    for (int ri = 0; ri < 3; ++ri) {
        int   n  = ring_counts[ri];
        float rr = radius_px * ring_fracs[ri];
        float dist2 = rr * rr;
        float w_spatial = _expf(-dist2 * inv2ss);
        float dtheta = 6.2831853f / (float)n;   // 2*pi / n

        for (int ti = 0; ti < 16; ++ti) {       // fixed upper bound (max ring)
            if (ti >= n) break;
            float theta = phase + dtheta * (float)ti;
            float ox = _cosf(theta) * rr;
            float oy = _sinf(theta) * rr;

            int sx = (int)(p_X + ox + 0.5f);
            int sy = (int)(p_Y + oy + 0.5f);
            if (sx < 0) sx = 0;
            if (sx > p_Width - 1) sx = p_Width - 1;
            if (sy < 0) sy = 0;
            if (sy > p_Height - 1) sy = p_Height - 1;

            float tr = _tex2D(p_TexR, sx, sy);
            float tg = _tex2D(p_TexG, sx, sy);
            float tb = _tex2D(p_TexB, sx, sy);

            float tsum = tr + tg + tb;
            float w_sim;
            float m_tap;
            if (tsum < 0.01f || center_valid == 0) {
                // Degenerate tap (or degenerate centre): treat as zero.
                w_sim = 0.0f;
                m_tap = 0.0f;
            } else {
                float gn_t = tg / tsum;
                float dn_t = (tr - tb) / tsum;
                float dgn = gn_t - gn_c;
                float ddn = dn_t - dn_c;
                w_sim = _expf(-(dgn * dgn + ddn * ddn) * inv2sc);
                m_tap = skin_mask_gated(tr, tg, tb, range, lo, hi);
            }

            float w = w_spatial * w_sim;
            wsum += w;
            msum += w * m_tap;
        }

        phase += POOL_GOLDEN;
    }

    // wsum >= 1 always (centre tap has weight 1), so no zero-divide guard needed.
    return msum / wsum;
}

// Resolve's parser requires the transform() signature on a single line.
__DEVICE__ float3 transform(int p_Width, int p_Height, int p_X, int p_Y, __TEXTURE__ p_TexR, __TEXTURE__ p_TexG, __TEXTURE__ p_TexB) {

    // Center pixel sampled once. These replace the old p_R/p_G/p_B working set.
    float p_R = _tex2D(p_TexR, p_X, p_Y);
    float p_G = _tex2D(p_TexG, p_X, p_Y);
    float p_B = _tex2D(p_TexB, p_X, p_Y);

    // Stable originals for Show Mask zone classification (must survive edits).
    float orig_r = p_R, orig_g = p_G, orig_b = p_B;

    float r = p_R, g = p_G, b = p_B;

    // --- Evenness: hue compression toward skin center ---
    // Uses a broader mask than other adjustments to catch outlier hues.
    // Deliberately UNPOOLED: Evenness is hue compression, not the source of the
    // Density graininess complaint, and pooling it would double the tap cost.
    if (p_evenness > 0.0f) {
        float cmask = skin_mask_gated(r, g, b, p_range + COMPRESS_EXTRA, p_lo_gate, p_hi_gate);

        if (cmask > 1e-6f) {
            float3 hsv = rgb_to_hsv(make_float3(r, g, b));

            float d = hsv.x - SKIN_HUE_CENTER;
            if (d > 0.5f) d -= 1.0f;
            if (d < -0.5f) d += 1.0f;

            d *= (1.0f - p_evenness * cmask);
            hsv.x = SKIN_HUE_CENTER + d;
            if (hsv.x < 0.0f) hsv.x += 1.0f;
            if (hsv.x > 1.0f) hsv.x -= 1.0f;

            float3 rgb_new = hsv_to_rgb(hsv);
            r = rgb_new.x;
            g = rgb_new.y;
            b = rgb_new.z;
        }
    }

    // --- Main mask (with value gates) ---
    // Own (per-pixel) mask, computed on the (possibly Evenness-edited) pixel.
    float m_own = skin_mask_gated(r, g, b, p_range, p_lo_gate, p_hi_gate);
    float mask = m_own;

    // --- Spatial pooling (Soften) ---
    // When Soften == 0 we skip pooling entirely: no taps, behaviour is
    // bit-identical to the per-pixel tool. The pooled value is soft-UNIONed
    // with the own mask so it can only fill holes, never erode edges.
    // m_pooled stays in scope: Show Mask reuses it rather than re-pooling.
    float m_pooled = 0.0f;
    if (p_soften > 0.0f) {
        float radius_px = p_soften * (float)p_Width * 0.004f;   // ~15px @ UHD
        m_pooled = pool_skin_mask(p_TexR, p_TexG, p_TexB,
                                  p_Width, p_Height, p_X, p_Y,
                                  r, g, b, m_own, radius_px,
                                  p_range, p_lo_gate, p_hi_gate);
        mask = 1.0f - (1.0f - m_own) * (1.0f - m_pooled);
    }

    // --- Separation gate mask ---
    // Separation INVERTS the mask, so its gate must come from a POOLED mask
    // (color-contrast.MD §3): inverting the raw per-pixel mask would not merely
    // skip grain-dithered borderline skin pixels, it would actively push them
    // toward teal — cyan freckles, the Density graininess bug weaponized.
    //
    // Soften's own radius cannot be trusted to supply that pooling: at
    // Soften = 0.01 the radius is sub-pixel (~0.15px @ UHD), every ring tap
    // rounds back onto the centre, and the gate silently degrades to the raw
    // per-pixel mask — i.e. nudging Soften off zero would WEAKEN the guard.
    // So the gate always pools at >= SEP_FALLBACK_SOFTEN, and reuses the main
    // pooled mask only once Soften has met that floor.
    //
    // Computed HERE, before the HSV adjustments, so the pool's centre chroma
    // reference is the same untouched pixel its taps read out of the texture.
    // `mask` is never overwritten, so Separation cannot perturb the skin-side
    // adjustments in either direction.
    float sep_mask = mask;
    if (p_separation > 0.0f && p_soften < SEP_FALLBACK_SOFTEN) {
        float sep_radius = SEP_FALLBACK_SOFTEN * (float)p_Width * 0.004f;
        float m_sep = pool_skin_mask(p_TexR, p_TexG, p_TexB,
                                     p_Width, p_Height, p_X, p_Y,
                                     r, g, b, m_own, sep_radius,
                                     p_range, p_lo_gate, p_hi_gate);
        sep_mask = 1.0f - (1.0f - m_own) * (1.0f - m_sep);
    }

    // Early exit if nothing to do and no overlays to render.
    // NOTE: this fires on the POOLED mask, so a pixel whose own mask is ~0 but
    // which receives a nonzero pooled adjustment is NOT skipped. Separation
    // targets exactly the mask≈0 pixels, so it must also hold the exit open.
    //
    // PERF: holding it open is not free, and Separation's cost is paid on the
    // BACKGROUND, not on skin. Soften == 0 normally means "no taps anywhere,
    // bit-identical to the per-pixel tool"; raising Separation revokes that,
    // because every background pixel now both misses this early exit and runs
    // the 36-tap gate pool above (108 texture fetches/px). In the band
    // 0 < Soften < SEP_FALLBACK_SOFTEN the frame pays for two independent
    // pools; at Soften >= SEP_FALLBACK_SOFTEN it is back to one. Judge
    // playback performance in Resolve with Separation at its intended dose —
    // benchmarking at Separation = 0 measures a different shader.
    if (mask < 1e-6f && p_separation <= 0.0f && p_show_mask == 0 && p_legend == 0)
        return make_float3(r, g, b);

    // --- Hue, Saturation, Density adjustments in HSV ---
    if (mask > 1e-6f) {
        float3 hsv = rgb_to_hsv(make_float3(r, g, b));

        // Hue rotation (slider direction matches legend: left = cyan, right = magenta)
        if (p_hue != 0.0f) {
            hsv.x -= mask * p_hue / 360.0f;
            if (hsv.x < 0.0f) hsv.x += 1.0f;
            if (hsv.x > 1.0f) hsv.x -= 1.0f;
        }

        // Saturation scale
        if (p_saturation != 0.0f) {
            hsv.y = _fmaxf(0.0f, hsv.y * (1.0f + mask * p_saturation));
        }

        // Density (value scale, positive = darker)
        if (p_density != 0.0f) {
            hsv.z = _fmaxf(0.0f, hsv.z * (1.0f - mask * p_density));
        }

        float3 adj = hsv_to_rgb(hsv);
        r = adj.x;
        g = adj.y;
        b = adj.z;
    }

    // --- Legend strip at bottom of frame (independent of Show Mask) ---
    // Resolve DCTL uses top-down pixel coords: p_Y=0 is the top row.
    // The bottom 7.5% of the frame becomes the strip.
    if (p_legend == 1) {
        float y_norm = (float)p_Y / (float)p_Height;
        if (y_norm >= 1.0f - LEGEND_STRIP_H) {
            float x_norm = (float)p_X / (float)p_Width;

            // Continuous gradient: x maps linearly to zone in [+1, -1] so the
            // strip reads left→right as GREEN → SKIN → MAG, matching the
            // labels and the Hue slider convention (left = green, right = mag).
            float zone_strip = 1.0f - x_norm * 2.0f;
            float3 skin_base = make_float3(SKIN_BASE_R, SKIN_BASE_G, SKIN_BASE_B);
            float3 chip = zone_color(zone_strip, skin_base);
            float lr = chip.x;
            float lg = chip.y;
            float lb = chip.z;

            int cell = p_Height / 100;
            if (cell < 2) cell = 2;
            int char_w = 6 * cell;
            int char_h = 7 * cell;

            int strip_top = (int)((1.0f - LEGEND_STRIP_H) * (float)p_Height);
            int strip_height = p_Height - strip_top;
            int block_w = p_Width / 3;
            int third = (int)(x_norm * 3.0f);
            if (third > 2) third = 2;
            int block_left = third * block_w;

            int text_len = (third == 0) ? 5 : 4;   // GREEN | SKIN | MAG.
            int text_w = text_len * char_w;
            int text_left = block_left + (block_w - text_w) / 2;
            int text_top = strip_top + (strip_height - char_h) / 2;

            if (p_Y >= text_top && p_Y < text_top + char_h) {
                int lx = p_X - text_left;
                if (lx >= 0 && lx < text_w) {
                    int ci = lx / char_w;
                    int cx = (lx % char_w) / cell;
                    int cy = (p_Y - text_top) / cell;
                    if (cx < 5) {
                        int ch = 0;
                        if (third == 0) {
                            if (ci == 0) ch = 'G';
                            else if (ci == 1) ch = 'R';
                            else if (ci == 2) ch = 'E';
                            else if (ci == 3) ch = 'E';
                            else ch = 'N';
                        } else if (third == 1) {
                            if (ci == 0) ch = 'S';
                            else if (ci == 1) ch = 'K';
                            else if (ci == 2) ch = 'I';
                            else ch = 'N';
                        } else {
                            if (ci == 0) ch = 'M';
                            else if (ci == 1) ch = 'A';
                            else if (ci == 2) ch = 'G';
                            else ch = '.';
                        }

                        int gmask = glyph_row(ch, cy);
                        if ((gmask >> (4 - cx)) & 1) {
                            lr = GLYPH_GREY; lg = GLYPH_GREY; lb = GLYPH_GREY;
                        }
                    }
                }
            }

            float3 legend_px = make_float3(lr, lg, lb);
            return legend_px;
        }
    }

    // --- Show Mask: three-zone false-color overlay ---
    // Zone color from ORIGINAL hues (stable classification);
    // mask strength from ADJUSTED pixels (shows slider effect via intensity)
    if (p_show_mask == 1) {
        // vmask mirrors the adjustment path: own mask, soft-unioned with the
        // pooled mask from the main pass. Taps read the original texture
        // either way, so re-pooling here would only change the centre chroma
        // reference — a negligible difference for double the tap cost.
        // (m_pooled is 0.0 when Soften == 0, making the union a no-op.)
        float vown = skin_mask_gated(r, g, b, p_range, p_lo_gate, p_hi_gate);
        float vmask = 1.0f - (1.0f - vown) * (1.0f - m_pooled);
        float3 hsv_adj = rgb_to_hsv(make_float3(r, g, b));
        float grey = hsv_adj.z;

        // Zone from original pixel hue — stable regardless of Hue slider
        float3 hsv_orig = rgb_to_hsv(make_float3(orig_r, orig_g, orig_b));
        float hd = hsv_orig.x - SKIN_HUE_CENTER;
        if (hd > 0.5f) hd -= 1.0f;
        if (hd < -0.5f) hd += 1.0f;

        float width = SKIN_HUE_WIDTH * p_range;
        float zone = (width > 1e-6f) ? hd / width : 0.0f;

        // Per-pixel zone tint — same closed-form mapping the legend uses.
        // Base is the pixel's own greyscale (V), so the overlay rides on the
        // image's local luminance and reads as natural false-colour.
        float3 grey_base = make_float3(grey, grey, grey);
        float3 zc = zone_color(zone, grey_base);

        // Square vmask for display: compresses marginal matches toward grey,
        // keeping peak-skin pixels brightly saturated for clearer visual feedback.
        float vd = vmask * vmask;
        float mr = grey + vd * (zc.x - grey);
        float mg = grey + vd * (zc.y - grey);
        float mb = grey + vd * (zc.z - grey);

        // NOTE: this returns BEFORE Separation, deliberately — the overlay is a
        // readout of the skin mask, not a preview of the graded result, so it
        // shows no teal push. The cost is an evaluation blind spot: the
        // protective moat (color-contrast.MD §3, §8 step 3 — warm wood/beige
        // within pooling radius of a face getting partially exempted) has no
        // false-colour representation and can only be judged by eye on the
        // graded image. If that proves too blind, the honest fix is a separate
        // Show Separation readout of (1 - sep_mask), not overloading this one
        // with a second meaning.
        return make_float3(mr, mg, mb);
    }

    // --- Separation: push non-skin hues toward the skin complement ---
    // Inverse-mask color contrast (design + hazards: color-contrast.MD).
    // sep_mask is pooled up top — see the gate comment there for why the
    // pooling is load-bearing rather than merely convenient.
    if (p_separation > 0.0f) {
        float3 hsv = rgb_to_hsv(make_float3(r, g, b));

        // Chroma weight keeps neutrals neutral — the push redistributes
        // existing color, never invents it. The V ramp re-derives the
        // deep-black guard: skin_mask_gated returns 0 for sum < 0.01, which
        // would otherwise invert to a FULL push on near-blacks. sum < 0.01
        // implies V <= 0.01, so this ramp closes that hole exactly, not
        // approximately.
        float w = p_separation * (1.0f - sep_mask)
                * smoothstep(0.08f, 0.35f, hsv.y)
                * smoothstep(0.01f, 0.05f, hsv.z);

        if (w > 1e-6f) {
            // The field is evaluated on the pixel's NATIVE hue, not its
            // adjusted one. Hue and Evenness rotate masked pixels off 28°, and
            // the repeller has to stay nailed to real skin: read the adjusted
            // hue instead and a partially-masked edge pixel drifts out of the
            // field's null and takes a real push, demoting the doubly-redundant
            // protection of color-contrast.MD §2 to single. The magnitude is
            // small — the Hue rotation is itself mask-scaled, so the field
            // offset and the (1-mask) weight peak at opposite ends and the
            // product tops out near 2.4° at mask ≈ 0.5, Hue = ±20 — but
            // anchoring is free, exact, and does not assume the Hue slider
            // keeps its current ±20 range. For non-skin pixels nothing is
            // adjusted, so native == current and this is a no-op.
            float3 hsv_native = rgb_to_hsv(make_float3(orig_r, orig_g, orig_b));
            float h_field = (hsv_native.y > 1e-6f) ? hsv_native.x : hsv.x;

            // Sinusoidal hue field: stable attractor at the complement,
            // unstable equilibrium at the skin hue itself (zero push there —
            // skin is protected by the field even before the mask gate), and
            // smooth everywhere on the circle, so adjacent hues never tear
            // apart at a watershed. Note the field is at its MAXIMUM at the
            // 90° flanks (118° green, 298° magenta); the point of zero push is
            // the repeller at 28°, which is exactly where naive shortest-path
            // rotation would tear.
            float dh = h_field - SEP_COMPLEMENT;
            if (dh > 0.5f) dh -= 1.0f;
            if (dh < -0.5f) dh += 1.0f;

            // KNOWN AND UNMITIGATED (color-contrast.MD §4.3): reds sit on the
            // magenta flank and drift the long way round. Crimson at 355° takes
            // -14.7° on an unmasked, saturated pixel at Separation = 1, landing
            // near 340° — the "why is her dress purple" failure, arriving at
            // roughly half the maximum dose. No protection notch yet, and that
            // is a decision, not an oversight: a notch flattens the field for
            // warm practicals and skin-adjacent wood/beige that this effect is
            // supposed to move. Evaluate on real footage before adding one.
            hsv.x -= w * SEP_FIELD_GAIN * _sinf(dh * 6.2831853f);
            if (hsv.x < 0.0f) hsv.x += 1.0f;
            if (hsv.x > 1.0f) hsv.x -= 1.0f;

            // Pure hue rotation at constant S/V stays in gamut, so this
            // needs no help from the mask-scaled squeeze below.
            float3 sep_rgb = hsv_to_rgb(hsv);
            r = sep_rgb.x;
            g = sep_rgb.y;
            b = sep_rgb.z;
        }
    }

    // Gamut squeeze scaled by mask. The squeeze exists to contain adjustment
    // overshoot, and adjustment magnitude already scales with mask — so
    // faintly pooled-in pixels (background within pooling radius of skin)
    // must not receive the full shoulder/toe, or Soften paints a squeeze halo
    // around skin against bright/dark backgrounds. Gain of 4 reaches full
    // squeeze by mask 0.25, keeping core-skin behaviour unchanged.
    float3 squeezed = soft_squeeze(make_float3(r, g, b));
    float sq = _fminf(mask * 4.0f, 1.0f);
    float3 result = make_float3(r + sq * (squeezed.x - r),
                                g + sq * (squeezed.y - g),
                                b + sq * (squeezed.z - b));
    return result;
}

}
#undef SKIN_HUE_CENTER
#undef SKIN_HUE_WIDTH
#undef COMPRESS_EXTRA
#undef LEGEND_STRIP_H
#undef SKIN_BASE_R
#undef SKIN_BASE_G
#undef SKIN_BASE_B
#undef GLYPH_GREY
#undef SEP_COMPLEMENT
#undef SEP_FIELD_GAIN
#undef SEP_FALLBACK_SOFTEN
#undef POOL_GOLDEN
int main(){ width=257; int height=17; int n=width*height; std::mt19937 rng(711); std::uniform_real_distribution<float> uni(0,1); std::vector<float> r(n),g(n),b(n),ar(n),ag(n),ab(n); double worst=0; long comparisons=0; 
for(int trial=0;trial<101;trial++){
if(trial==50){width=3840;height=3;n=width*height;r.resize(n);g.resize(n);b.resize(n);ar.resize(n);ag.resize(n);ab.resize(n);}
for(int i=0;i<n;i++){r[i]=uni(rng)*1.4f-0.2f;g[i]=uni(rng)*1.4f-0.2f;b[i]=uni(rng)*1.4f-0.2f;}
merged::enable_K = (trial==0 || trial==100) ? 1 : (uni(rng)>0.5f);
merged::enable_S = (trial==0 || trial==100) ? 1 : (uni(rng)>0.5f);
merged::enable_H = (trial==0 || trial==100) ? 1 : (uni(rng)>0.5f);
merged::enable_P = (trial==0 || trial==100) ? 1 : (uni(rng)>0.5f);
merged::P_p_exposure = (trial==0 || trial==100) ? 0.0f : ( -6.0f + uni(rng)*( 6.0f-( -6.0f)));
merged::P_p_bp = (trial==0 || trial==100) ? 0.0f : ( -0.05f + uni(rng)*( 0.05f-( -0.05f)));
merged::P_p_temp = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::P_p_tint = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::P_p_contrast = (trial==0 || trial==100) ? 1.0f : ( 0.5f + uni(rng)*( 2.0f-( 0.5f)));
merged::P_p_pivot_offset = (trial==0 || trial==100) ? 0.0f : ( -0.2f + uni(rng)*( 0.2f-( -0.2f)));
merged::P_p_shadows = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::P_p_highlights = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::P_p_high_soft = (trial==0 || trial==100) ? 0.0f : ( 0.0f + uni(rng)*( 2.0f-( 0.0f)));
merged::P_p_neg_sat = (trial==0 || trial==100) ? 1.0f : ( 0.0f + uni(rng)*( 1.0f-( 0.0f)));
merged::P_p_pos_sat = (trial==0 || trial==100) ? 0.0f : ( 0.0f + uni(rng)*( 1.0f-( 0.0f)));
merged::P_p_preserve_lum = (trial==0 || trial==100) ? 0 : (uni(rng)>0.5f);
merged::P_p_show_chart = (trial==0 || trial==100) ? 0 : (uni(rng)>0.5f);
merged::H_p_red_hue = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_red_den = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_yel_hue = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_yel_den = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_grn_hue = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_grn_den = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_cyn_hue = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_cyn_den = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_blu_hue = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_blu_den = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_mag_hue = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_mag_den = (trial==0 || trial==100) ? 0.0f : ( -1.0f + uni(rng)*( 1.0f-( -1.0f)));
merged::H_p_preserve_lum = (trial==0 || trial==100) ? 0 : (uni(rng)>0.5f);
merged::H_p_soft_squeeze = (trial==0 || trial==100) ? 1 : (uni(rng)>0.5f);
merged::H_p_hard_squeeze = (trial==0 || trial==100) ? 0 : (uni(rng)>0.5f);
merged::H_p_cinecolor = (trial==0 || trial==100) ? 0.0f : ( 0.0f + uni(rng)*( 1.0f-( 0.0f)));
merged::H_p_protect_skin = (trial==0 || trial==100) ? 0 : (uni(rng)>0.5f);
merged::S_pivot = (trial==0 || trial==100) ? 0.0f : ( -0.2f + uni(rng)*( 0.2f-( -0.2f)));
merged::S_transition = (trial==0 || trial==100) ? 0.2f : ( 0.01f + uni(rng)*( 1.0f-( 0.01f)));
merged::S_shadow_red = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_shadow_green = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_shadow_blue = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_shadow_cyan = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_shadow_magenta = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_shadow_yellow = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_highlight_red = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_highlight_green = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_highlight_blue = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_highlight_cyan = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_highlight_magenta = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_highlight_yellow = (trial==0 || trial==100) ? 0.0f : ( -0.25f + uni(rng)*( 0.25f-( -0.25f)));
merged::S_preserve_luminance = (trial==0 || trial==100) ? 0.8f : ( 0.0f + uni(rng)*( 1.0f-( 0.0f)));
merged::S_opt_showcurve = (trial==0 || trial==100) ? 0 : (uni(rng)>0.5f);
merged::S_visualize = (trial==0 || trial==100) ? 0 : (uni(rng)>0.5f);
merged::S_opt_showramp = (trial==0 || trial==100) ? 0 : (uni(rng)>0.5f);
merged::S_ramp_pos = (trial==0 || trial==100) ? 0.0f : ( 0.0f + uni(rng)*( 0.85f-( 0.0f)));
merged::S_p_show_chart = (trial==0 || trial==100) ? 0 : (uni(rng)>0.5f);
merged::K_p_hue = (trial==0 || trial==100) ? 0.0f : ( -20.0f + uni(rng)*( 20.0f-( -20.0f)));
merged::K_p_saturation = (trial==0 || trial==100) ? 0.0f : ( -0.5f + uni(rng)*( 0.5f-( -0.5f)));
merged::K_p_density = (trial==0 || trial==100) ? 0.0f : ( -0.2f + uni(rng)*( 0.2f-( -0.2f)));
merged::K_p_range = (trial==0 || trial==100) ? 1.0f : ( 0.25f + uni(rng)*( 2.0f-( 0.25f)));
merged::K_p_soften = (trial==0 || trial==100) ? 0.0f : ( 0.0f + uni(rng)*( 1.0f-( 0.0f)));
merged::K_p_evenness = (trial==0 || trial==100) ? 0.0f : ( 0.0f + uni(rng)*( 1.0f-( 0.0f)));
merged::K_p_separation = (trial==0 || trial==100) ? 0.0f : ( 0.0f + uni(rng)*( 1.0f-( 0.0f)));
merged::K_p_lo_gate = (trial==0 || trial==100) ? 0.0f : ( 0.0f + uni(rng)*( 0.5f-( 0.0f)));
merged::K_p_hi_gate = (trial==0 || trial==100) ? 1.0f : ( 0.5f + uni(rng)*( 1.0f-( 0.5f)));
merged::K_p_show_mask = (trial==0 || trial==100) ? 0 : (uni(rng)>0.5f);
merged::K_p_legend = (trial==0 || trial==100) ? 0 : (uni(rng)>0.5f);
if(trial<50){merged::P_p_show_chart=0;merged::S_p_show_chart=0;merged::S_opt_showcurve=0;merged::S_visualize=0;merged::S_opt_showramp=0;merged::K_p_show_mask=0;merged::K_p_legend=0;}
if(trial<16){merged::enable_P=trial&1;merged::enable_H=(trial>>1)&1;merged::enable_S=(trial>>2)&1;merged::enable_K=(trial>>3)&1;}
r[0]=g[0]=b[0]=0; r[1]=g[1]=b[1]=0.336043f; r[2]=g[2]=b[2]=1; r[3]=1;g[3]=b[3]=0; r[4]=0;g[4]=1;b[4]=0; r[5]=g[5]=0;b[5]=1;
Primera::p_exposure=merged::P_p_exposure;
Primera::p_bp=merged::P_p_bp;
Primera::p_temp=merged::P_p_temp;
Primera::p_tint=merged::P_p_tint;
Primera::p_contrast=merged::P_p_contrast;
Primera::p_pivot_offset=merged::P_p_pivot_offset;
Primera::p_shadows=merged::P_p_shadows;
Primera::p_highlights=merged::P_p_highlights;
Primera::p_high_soft=merged::P_p_high_soft;
Primera::p_neg_sat=merged::P_p_neg_sat;
Primera::p_pos_sat=merged::P_p_pos_sat;
Primera::p_preserve_lum=merged::P_p_preserve_lum;
Primera::p_show_chart=merged::P_p_show_chart;
PrimeraHue::p_red_hue=merged::H_p_red_hue;
PrimeraHue::p_red_den=merged::H_p_red_den;
PrimeraHue::p_yel_hue=merged::H_p_yel_hue;
PrimeraHue::p_yel_den=merged::H_p_yel_den;
PrimeraHue::p_grn_hue=merged::H_p_grn_hue;
PrimeraHue::p_grn_den=merged::H_p_grn_den;
PrimeraHue::p_cyn_hue=merged::H_p_cyn_hue;
PrimeraHue::p_cyn_den=merged::H_p_cyn_den;
PrimeraHue::p_blu_hue=merged::H_p_blu_hue;
PrimeraHue::p_blu_den=merged::H_p_blu_den;
PrimeraHue::p_mag_hue=merged::H_p_mag_hue;
PrimeraHue::p_mag_den=merged::H_p_mag_den;
PrimeraHue::p_preserve_lum=merged::H_p_preserve_lum;
PrimeraHue::p_soft_squeeze=merged::H_p_soft_squeeze;
PrimeraHue::p_hard_squeeze=merged::H_p_hard_squeeze;
PrimeraHue::p_cinecolor=merged::H_p_cinecolor;
PrimeraHue::p_protect_skin=merged::H_p_protect_skin;
PrimeraSplit::pivot=merged::S_pivot;
PrimeraSplit::transition=merged::S_transition;
PrimeraSplit::shadow_red=merged::S_shadow_red;
PrimeraSplit::shadow_green=merged::S_shadow_green;
PrimeraSplit::shadow_blue=merged::S_shadow_blue;
PrimeraSplit::shadow_cyan=merged::S_shadow_cyan;
PrimeraSplit::shadow_magenta=merged::S_shadow_magenta;
PrimeraSplit::shadow_yellow=merged::S_shadow_yellow;
PrimeraSplit::highlight_red=merged::S_highlight_red;
PrimeraSplit::highlight_green=merged::S_highlight_green;
PrimeraSplit::highlight_blue=merged::S_highlight_blue;
PrimeraSplit::highlight_cyan=merged::S_highlight_cyan;
PrimeraSplit::highlight_magenta=merged::S_highlight_magenta;
PrimeraSplit::highlight_yellow=merged::S_highlight_yellow;
PrimeraSplit::preserve_luminance=merged::S_preserve_luminance;
PrimeraSplit::opt_showcurve=merged::S_opt_showcurve;
PrimeraSplit::visualize=merged::S_visualize;
PrimeraSplit::opt_showramp=merged::S_opt_showramp;
PrimeraSplit::ramp_pos=merged::S_ramp_pos;
PrimeraSplit::p_show_chart=merged::S_p_show_chart;
PrimeraSkin::p_hue=merged::K_p_hue;
PrimeraSkin::p_saturation=merged::K_p_saturation;
PrimeraSkin::p_density=merged::K_p_density;
PrimeraSkin::p_range=merged::K_p_range;
PrimeraSkin::p_soften=merged::K_p_soften;
PrimeraSkin::p_evenness=merged::K_p_evenness;
PrimeraSkin::p_separation=merged::K_p_separation;
PrimeraSkin::p_lo_gate=merged::K_p_lo_gate;
PrimeraSkin::p_hi_gate=merged::K_p_hi_gate;
PrimeraSkin::p_show_mask=merged::K_p_show_mask;
PrimeraSkin::p_legend=merged::K_p_legend;
Primera::p_tf=5; PrimeraSplit::p_tf=5;
for(int i=0;i<n;i++){int x=i%width,y=i/width;float3 c={r[i],g[i],b[i]};
if(merged::enable_P)c=Primera::transform(width,height,x,y,c.x,c.y,c.z);
if(merged::enable_H)c=PrimeraHue::transform(width,height,x,y,c.x,c.y,c.z);
if(merged::enable_S)c=PrimeraSplit::transform(width,height,x,y,c.x,c.y,c.z);
ar[i]=c.x;ag[i]=c.y;ab[i]=c.z;}
for(int i=0;i<n;i++){int x=i%width,y=i/width;float3 ref={ar[i],ag[i],ab[i]};if(merged::enable_K)ref=PrimeraSkin::transform(width,height,x,y,ar.data(),ag.data(),ab.data());
float3 got=merged::transform(width,height,x,y,r.data(),g.data(),b.data());
float aa[]={got.x,got.y,got.z},bb[]={ref.x,ref.y,ref.z};for(int k=0;k<3;k++){assert(std::isfinite(aa[k]));assert(std::isfinite(bb[k]));double e=fabs(aa[k]-bb[k]);worst=std::max(worst,e);assert(e<1e-6);comparisons++;}
merged::bypass=1;got=merged::transform(width,height,x,y,r.data(),g.data(),b.data());assert(got.x==r[i]&&got.y==g[i]&&got.z==b[i]);merged::bypass=0;
}}
printf("PASS: %ld channel comparisons, maximum absolute difference %.9g; bypass exact.\n",comparisons,worst);
}
