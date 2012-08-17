class IOManager
{
private:
	static IOManager* _ioManager;
protected:
	IOManager();;
public:
	static IOManager* Instance();

public:
	const char* GetInputHandlerTypes();
};