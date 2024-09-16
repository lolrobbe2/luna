#pragma once
#ifndef _SCRIPT_LANGUAGE_
#define _SCRIPT_LANGUAGE_
#include <core/object/object.h>
namespace luna 
{

	namespace scripting 
	{
		class scriptInstance : public object 
		{
			LN_CLASS(scriptInstance, object);
			virtual void call() = 0;
		};
		class script : public object
		{
			LN_CLASS(script, object);
			virtual void instantiate() = 0;
		};

		class scriptLanguage : public object
		{
		public:
			virtual std::string getName() const = 0;

			virtual void init() = 0;
			virtual std::string getType() const = 0;
			virtual std::string getExtension() const = 0;
			virtual void finish() = 0;
		private:

		};

		scriptLanguage::scriptLanguage()
		{
		}

		scriptLanguage::~scriptLanguage()
		{
		}
	}
}
#endif // !_SCRIPT_LANGUAGE_
