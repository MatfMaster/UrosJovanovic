#include <QDateTime>
#include "RenderStats.h"

#define MAX_FRAMES 20

long GameEngine::FrameStats::_frameCounter = 0;

GameEngine::FrameStats::FrameStats()
	: _id(-1),
	  _start(0),
	  _time(0),
	  _drawCalls(0) {}

GameEngine::FrameStats::FrameStats(int time)
	: FrameStats(time, 0) {}

GameEngine::FrameStats::FrameStats(int time, int drawCalls)
	: _id(_frameCounter++),
	  _start(QDateTime::currentMSecsSinceEpoch()),
	  _time(time),
	  _drawCalls(drawCalls) {}

long GameEngine::FrameStats::id() const
{
	return _id;
}

long GameEngine::FrameStats::start() const
{
	return _start;
}

double GameEngine::FrameStats::time() const
{
	return _time;
}

int GameEngine::FrameStats::drawCalls() const
{
	return _drawCalls;
}

void GameEngine::FrameStats::setTime(double time)
{
	_time = time;
}

void GameEngine::FrameStats::incrementDrawCalls()
{
	_drawCalls++;
}

GameEngine::RenderStats::RenderStats()
	: _currFrame(0),
	  _batchCount(0),
	  _batchSize(0),
	  _avgFPS(0),
	  _frameStats(MAX_FRAMES),
	  _fCullStatus(false)
{
	_frameStats[_currFrame] = FrameStats(0);
}

float GameEngine::RenderStats::averageFrameTime() const
{
	double total = 0;
	for (auto frameStats : _frameStats)
		total += frameStats.time();
	return total * 1.0f / MAX_FRAMES;
}

float GameEngine::RenderStats::averageFrameRate() const
{
	return _avgFPS;
}

float GameEngine::RenderStats::averageDrawCalls() const
{
	int total = 0;
	for (auto frameStats : _frameStats)
		total += frameStats.drawCalls();
	return total * 1.0f / MAX_FRAMES;
}

int GameEngine::RenderStats::batchCount() const
{
	return _batchCount;
}

GameEngine::FrameStats& GameEngine::RenderStats::currentFrame()
{
	return _frameStats[_currFrame];
}

void GameEngine::RenderStats::pushCurrentFrame()
{
	auto cur = currentFrame();
	auto delta = fabsf(cur.start() - _lastSync.start());
	if (delta >= 1000)
	{
		auto elapsedFrames = cur.id() - _lastSync.id();
		_avgFPS = 1000.0f / delta * elapsedFrames;
		_lastSync = cur;
	}
	_frameStats[++_currFrame %= MAX_FRAMES] = FrameStats(0);
}

int GameEngine::RenderStats::batchSize() const
{
	return _batchSize;
}

bool GameEngine::RenderStats::getFrustumCullStatus() const
{
	return _fCullStatus;
}

void GameEngine::RenderStats::setBatchCount(int count)
{
	_batchCount = count;
}

void GameEngine::RenderStats::setBatchSize(int size)
{
	_batchSize = size;
}

void GameEngine::RenderStats::setFrustumCullStatus(bool status)
{
	_fCullStatus = status;
}

QString GameEngine::RenderStats::toQString() const
{
	char* unit = "bytes";
	float bSize = batchSize();
	if (bSize > 1024)
	{
		bSize /= 1024.0f;
		unit = "KB";
	}
	if (bSize > 1024)
	{
		bSize /= 1024.0f;
		unit = "MB";
	}
	bSize = bSize > 1024 ? bSize / 1024 : bSize;
	bSize = bSize > 1024 ? bSize / 1024 : bSize;
	return QString::asprintf("Frustum Culling: %s; %.0f draw calls @ %.0f FPS (%.2fms); %i batches (%.2f %s)",
	                         _fCullStatus ? "ON" : "OFF", averageDrawCalls(), averageFrameRate(), averageFrameTime(), batchCount(), bSize, unit);
}
