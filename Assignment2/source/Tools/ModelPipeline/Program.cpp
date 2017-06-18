#include "pch.h"

using namespace std;
using namespace ModelPipeline;
using namespace Library;

int main(int argc, char* argv[])
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		if (argc < 2)
		{
			throw exception("Usage: ...TODO");
		}

		string inputFile = argv[1];
		string inputFilename;
		string inputDirectory;			
		Library::Utility::GetFileNameAndDirectory(inputFile, inputDirectory, inputFilename);
		if (inputDirectory.empty())
		{
			inputDirectory = UtilityWin32::CurrentDirectory();
		}

		SetCurrentDirectory(Library::Utility::ToWideString(inputDirectory).c_str());		
		Model model = ModelProcessor::LoadModel(inputFilename, true);
		
		string outputFilename = inputFilename + ".bin";		
		model.Save(outputFilename);
	}
	catch (exception ex)
	{
		cout << ex.what();
	}

	return 0;
}