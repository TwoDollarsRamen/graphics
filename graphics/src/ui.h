#pragma once

#include "common.h"
#include "renderer2d.h"

struct ui;

struct ui* new_ui_ctx(struct renderer2d* renderer, struct font* font);
void free_ui_ctx(struct ui* ctx);

void ui_begin_frame(struct ui* ctx, v2f mouse_pos, bool mouse_clicked);
void ui_end_frame(struct ui* ctx);
void ui_text(struct ui* ctx, const char* text);
void ui_text_ex(struct ui* ctx, const char* text, v3f color);
bool ui_button(struct ui* ctx, const char* text);

bool ui_anything_hovered(struct ui* ctx);
