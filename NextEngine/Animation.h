#pragma once

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Transform.h"
#include <string>
#include <map>

using namespace std;

namespace AnimationData {
	constexpr float DEFAULT_TIME_PER_FRAME = 0.12f;
	//constexpr float DEFAULT_TIME_PER_FRAME = 0.5f;
}

class Animation {
	public:
		/// @brief The struct for an animation state
		/// 
		/// The struct contains the information about the state to be used by the animator within Animation
		struct State {
			State() : name(""), row(-1), frameCount(1) {}

			/// @param name the name of the state
			/// @param row the number of row in the source file of the animation
			/// @param frameCount the number of frames in the animation
			State(string name, int row, int startCol, int frameCount, bool canLoop, float timePerFrame) : name(name), row(row), startCol(startCol), 
				frameCount(frameCount), currentFrame(0), canLoop(canLoop), isPlaying(canLoop), timePerFrame(timePerFrame) {}
			string name;
			int row;
			int startCol;
			int frameCount;
			int currentFrame;
			bool canLoop;
			bool isPlaying;
			float timePerFrame;
		};

	private:
		//Animator
		map<string, State> states;
		State* currentState;

		// Animation
		unsigned int& texture;
		int rowCount;
		int colCount;
		float offsetX;
		float offsetY;
		float timeSinceLastFrame;
		float timePerFrame;
		bool isPaused;

	public:
		//Animator

		/// @brief This function adds a new Animation::State to the animation
		/// @param name is the name of the state
		/// @param row is the number of row in the source file of the animation
		/// @param frameCount is the number of frames in the animation
		void addState(string name, int row, int startCol, int frameCount, bool canLoop);
		void addState(string name, int row, int startCol, int frameCount, bool canLoop, float customTimePerFrame);

		/// @brief This function sets the current state of the animation
		/// The function plays a new animation that is defined by the name of the state.
		/// When the name does not match any existing state it will print the error message to the console.
		/// @param name is the name of the state
		void setState(string name);
		void updateCurrentState();

		//Animation
		
		/// @brief The constructor for animation
		/// @param texture is the render data from the GLRenderer
		Animation(unsigned int& texture);

		/// @brief This function renders the animation to the screen
		/// @param globalModelTransform a 4x4 matrix containing the global transform
		/// @param transform is the transformation for the render
		void render(glm::mat4 globalModelTransform, Transform& transform);

		/// @brief This function sets the current frame according to its position on the animation file.
		/// @param row is the row of the frame
		/// @param column is the column of the frame
		void setFrame(int row, int column);

		/// @brief This function sets the number of row and column of the animation
		/// @param row is the row of the frame
		/// @param col is the column of the frame
		void setDimension(int row, int col);

		void setTimePerFrame(float t);
		
		/// @brief This function sets the current frame to the random frame of the current animation
		void setRandomFrame();
		void setPaused(bool value);

		int getCurrentFrame() const;
		bool getIsPlaying() const;
		State getCurrentAnimationState() const;
		float getTimeRate() const;
};