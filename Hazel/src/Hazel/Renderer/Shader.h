#pragma once

#include <string>

namespace Hazel {

	class Shader
	{
	public:
		enum ShaderType
		{
			VERTEX, 
			FRAGMENT
		};

		virtual ~Shader(){};

		virtual void compile(const std::string& source) = 0;

		static inline std::string getShaderWithLineInfo(const std::string& source) {
			std::string res = "1 ";
			int line = 2;
			for (auto c : source) {
				res += c;
				if (c == '\n') {
					res += std::to_string(line) + ' ';
					line++;
				}
			}
			res += '\n';
			return res;
		}
	};

}