#pragma once
#include <QVector>

namespace GameEngine {
	class FrameStats final
	{
	public:
		FrameStats();
		FrameStats(int time);
		FrameStats(int time, int drawCalls);
		long id() const;
		long start() const;
		double time() const;
		int drawCalls() const;
		void setTime(double time);
		void incrementDrawCalls();

	private:
		static long _frameCounter;
		long _id;
		long _start;
		double _time;
		int _drawCalls;
	};

	class RenderStats final
	{
	public:
		RenderStats();
		float averageFrameTime() const;
		float averageFrameRate() const;
		float averageDrawCalls() const;
		int batchCount() const;
		int batchSize() const;
		bool getFrustumCullStatus() const;
		FrameStats& currentFrame();
		void pushCurrentFrame();
		void setBatchCount(int count);
		void setBatchSize(int size);
		void setFrustumCullStatus(bool status);
		QString toQString() const;

	private:
		int _currFrame;
		int _batchCount;
		int _batchSize;
		float _avgFPS;
		bool _fCullStatus;
		FrameStats _lastSync;
		QVector<FrameStats> _frameStats;
	};
}
