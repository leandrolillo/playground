/*
 * audioRunner.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#pragma once
#include <Math3d.h>
#include "Playground.h"
#include "OggResourceAdapter.h"
#include "WavResourceAdapter.h"
#include "AudioSource.h"

class AudioRunner: public PlaygroundRunner {
	public:
		static const unsigned char ID;
	public:
		using PlaygroundRunner::PlaygroundRunner; //inherit constructors

		virtual unsigned char getId() const override {
			return ID;
		}

		virtual bool initialize() override {
			this->getResourceManager().addAdapter<OggResourceAdapter>();
			this->getResourceManager().addAdapter<WavResourceAdapter>();
			return true;
		}

		virtual AudioSource *createSource(String fileName, const vector &position = vector(0, 0, 0), const vector &velocity = vector(0, 0, 0), bool loop = true) = 0;
		virtual void updateSource(AudioSource &source) = 0;
		virtual void playSource(AudioSource &source) = 0;
		virtual void stopSource(AudioSource &source) = 0;
		virtual void pauseSource(AudioSource &source) = 0;

		virtual bool updateListener(vector position, vector velocity = vector(0, 0, 0), vector to = vector(0, 0, 1), vector up = vector(0, 1, 0)) = 0;

		virtual String toString() const override {
			return "AudioRunner(id:" + std::to_string(this->getId()) + ")";
		}


		virtual ~AudioRunner()
		{
		}
	};
