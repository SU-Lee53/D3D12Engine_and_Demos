#pragma once


class Application
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

};

