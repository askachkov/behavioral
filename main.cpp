#include "factory.h"
#include "adapter.h"
#include "flyweight.h"
#include "app.h"
#include "config.h"

int main(int argc, char* argv[])
{
	Config::init(Config::ReaderPtr(new FileReader));
	ConsoleDrawer d;
	CashNodeFactory f;
	App app;
	app.setDrawer(&d);
	app.setFactory(&f);
	app.prepare();
	return app.exec();
}
