//
// NestedDiagnosticContext.cpp
//
// Library: Foundation
// Package: Core
// Module:  NestedDiagnosticContext
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/NestedDiagnosticContext.h"
#include "Poco/Path.h"


namespace Poco {


NestedDiagnosticContext::NestedDiagnosticContext()
{
}


NestedDiagnosticContext::NestedDiagnosticContext(const NestedDiagnosticContext& ctx)
{
	_stack = ctx._stack;
}


NestedDiagnosticContext::~NestedDiagnosticContext()
{
}


NestedDiagnosticContext& NestedDiagnosticContext::operator = (const NestedDiagnosticContext& ctx)
{
	if (&ctx != this)
		_stack = ctx._stack;
	return *this;
}


void NestedDiagnosticContext::push(const std::string& info)
{
	Context ctx;
	ctx.info = info;
	ctx.line = -1;
	ctx.file = 0;
	_stack.push_back(ctx);
}


void NestedDiagnosticContext::push(const std::string& info, LineNumber line, const char* filename)
{
	Context ctx;
	ctx.info = info;
	ctx.line = line;
	ctx.file = filename;
	_stack.push_back(ctx);
}


void NestedDiagnosticContext::pop()
{
	if (!_stack.empty())
		_stack.pop_back();
}


int NestedDiagnosticContext::depth() const
{
	return int(_stack.size());
}


std::string NestedDiagnosticContext::toString() const
{
	std::string result;
	for (const auto& i: _stack)
	{
		if (!result.empty())
			result.append(":");
		result.append(i.info);
	}
	return result;
}


void NestedDiagnosticContext::dump(std::ostream& ostr) const
{
	dump(ostr, "\n");
}


void NestedDiagnosticContext::dump(std::ostream& ostr, const std::string& delimiter, bool nameOnly) const
{
	for (auto it = _stack.begin(); it != _stack.end(); ++it)
	{
		if (it != _stack.begin())
		{
			ostr << delimiter;
		}

		std::string file = it->file ? it->file : "";
		if (nameOnly && !file.empty())
		{
			file = Path(file).getFileName();
		}

		ostr << it->info;
		if (!file.empty())
		{
			ostr << " (in \"" << file << "\", line " << it->line << ")";
		}
	}
}


void NestedDiagnosticContext::clear()
{
	_stack.clear();
}


NestedDiagnosticContext& NestedDiagnosticContext::current()
{
	static thread_local NestedDiagnosticContext ndc;
	return ndc;
}


} // namespace Poco
