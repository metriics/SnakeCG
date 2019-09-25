//////////////////////////////////////////////////////////////////////////
//
// This file is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this file in your GDW games.
//
// This file contains the implementations for the exposed GraphicsUtils
// API of TTK
//
// Based off of TTK by Michael Gharbharan 2017
// Shawn Matthews 2019
//
//////////////////////////////////////////////////////////////////////////

#include "GraphicsUtils.h"
#include "TTKContext.h"
#include <GLM/gtc/matrix_transform.inl>

// Helpers to convert raw pointers to glm types, and some colors
#define BLACK glm::vec4(0, 0, 0, 1)
#define WHITE glm::vec4(1, 1, 1, 1)
#define VEC3(ptr) *(glm::vec3*)ptr
#define VEC4(ptr) *(glm::vec4*)ptr
#define DEFAULT_BLACK(ptr) ptr == nullptr ? BLACK : VEC4(ptr)
#define DEFAULT_WHITE(ptr) ptr == nullptr ? WHITE : VEC4(ptr)

void TTK::Graphics::SetCameraMode2D(int windowWidth, int windowHeight) {
	TTK::Context::Instance().SetWindowSize(windowWidth, windowHeight);
	TTK::Context::Instance().SetProjection(glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -100.0f, 100.0f));
}

void TTK::Graphics::SetCameraMode3D(int windowWidth, int windowHeight) {
	TTK::Context::Instance().SetWindowSize(windowWidth, windowHeight);
	TTK::Context::Instance().SetProjection(glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.001f, 1000.0f));
}

void TTK::Graphics::SetDepthEnabled(bool isEnabled) {
	if (isEnabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void TTK::Graphics::SetCameraMatrix(const glm::mat4& view) {
	TTK::Context::Instance().SetView(view);
}

void TTK::Graphics::SetBackgroundColour(float r, float g, float b) {
	glClearColor(r, g, b, 1.0f);
}

void TTK::Graphics::ClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void TTK::Graphics::EndFrame() {
	TTK::Context::Instance().Flush();
}

void TTK::Graphics::DrawGrid(float gridWidth, AlignMode mode) {
	// Note: since we want to support both Y-Up and Z-Up, our points are a little
	// more complex. We set the y in z-up mode, or z in y-up mode. We use ternary
	// operators to keep the code from getting large
	const float gridMin{ -10.0f * gridWidth }, gridMax{ 10.0f * gridWidth };
	for(int x = -10; x <= 10; x++) {
		TTK::Context::Instance().AddLine(
			{ x * gridWidth, mode == ZUp ? gridMin : 0, mode == YUp ? gridMin : 0 },
			{ x * gridWidth, mode == ZUp ? gridMax : 0, mode == YUp ? gridMax : 0 },
			x == 0 ? glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		for(int y = -10; y <= 10; y++) {
			TTK::Context::Instance().AddLine(
				{ gridMin, mode == ZUp ? y * gridWidth : 0, mode == YUp ? y * gridWidth : 0 },
				{ gridMax, mode == ZUp ? y * gridWidth : 0, mode == YUp ? y * gridWidth : 0 },
				y == 0 ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}
}

void TTK::Graphics::Cleanup() {
	TTK::Context::DestroyContext();
	TTK::FontRenderer::DestroyContext();
}

void TTK::Graphics::DrawText2D(const std::string& text, int posX, int posY, int fontSize) {
	TTK::Context::Instance().RenderText(text.c_str(), { posX, posY }, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), fontSize / 32.0f);
}

void TTK::Graphics::DrawLine(const glm::vec3& p0, const glm::vec3& p1, float lineWidth, const glm::vec4& colour) {
	TTK::Context::Instance().AddLine(p0, p1, colour);
}

void TTK::Graphics::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& colour) {
	TTK::Context::Instance().AddLine(p0, p1, colour);
}

void TTK::Graphics::DrawLine(float* p0, float* p1, float lineWidth, float* colour) {
	TTK::Context::Instance().AddLine(VEC3(p0), VEC3(p1), DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawLine(float* p0, float* p1, float* colour) {
	TTK::Context::Instance().AddLine(VEC3(p0), VEC3(p1), DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawVector(const glm::vec3& origin, const glm::vec3& vectorToDraw, float lineWidth, const glm::vec4& colour) {
	TTK::Context::Instance().AddLine(origin, origin + glm::normalize(vectorToDraw), colour);
}

void TTK::Graphics::DrawVector(const glm::vec3& origin, const glm::vec3& vectorToDraw, const glm::vec4& colour) {
	TTK::Context::Instance().AddLine(origin, origin + glm::normalize(vectorToDraw), colour);
}

void TTK::Graphics::DrawVector(float* p0, float* v0, float lineWidth, float* colour) {
	DrawVector(VEC3(p0), VEC3(v0), lineWidth, DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawVector(float* origin, float* vectorToDraw, float* colour) {
	DrawVector(VEC3(origin), VEC3(vectorToDraw), DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawPoint(const glm::vec3& p0, float pointSize, const glm::vec4& colour) {
	TTK::Context::Instance().AddPoint(p0, pointSize, colour);
}

void TTK::Graphics::DrawPoint(float* p0, float pointSize, float* colour) {
	DrawPoint(VEC3(p0), pointSize, DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawCube(const glm::vec3& p0, float size, const glm::vec4& colour) {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), p0) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
	TTK::Context::Instance().DrawCube(transform, colour);
}

void TTK::Graphics::DrawCube(float* p0, float size, float* colour) {
	DrawCube(VEC3(p0), size, DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawTeapot(const glm::vec3& p0, float size, const glm::vec4 colour)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), p0) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
	TTK::Context::Instance().DrawTeapot(transform, colour);
}

void TTK::Graphics::DrawTeapot(float* p0, float size, float* colour)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), *(glm::vec3*)p0) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
	TTK::Context::Instance().DrawTeapot(transform, DEFAULT_BLACK(colour));
}

void TTK::Graphics::DrawSphere(const glm::vec3& center, float size, const glm::vec4& colour) {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
	TTK::Context::Instance().DrawSphere(transform, colour);
}

void TTK::Graphics::DrawSphere(const glm::mat4& p0, float size, const glm::vec4& colour) {
	TTK::Context::Instance().DrawSphere(p0, colour);
}

void TTK::Graphics::DrawSphere(const glm::mat4& transform, const glm::vec4& colour) {
	TTK::Context::Instance().DrawSphere(transform, colour);
}

void TTK::Graphics::DrawSphere(float* p0, float size, float* colour) {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), *(glm::vec3*)p0) * glm::scale(glm::mat4(1.0f), glm::vec3(size));
	TTK::Context::Instance().DrawSphere(transform, DEFAULT_BLACK(colour));
}
