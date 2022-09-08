#pragma once

namespace Hazel {

	struct DisableCopy
	{
		DisableCopy() = default;
		DisableCopy(const DisableCopy&) = delete;
		DisableCopy(DisableCopy&&) = delete;
		DisableCopy& operator=(const DisableCopy&) = delete;
		DisableCopy& operator=(DisableCopy&&) = delete;
	};

	struct DisableCopyAllowMove
	{
		DisableCopyAllowMove() = default;
		DisableCopyAllowMove(const DisableCopyAllowMove&) = delete;
		DisableCopyAllowMove(DisableCopyAllowMove&&) = default;
		DisableCopyAllowMove& operator=(const DisableCopyAllowMove&) = delete;
		DisableCopyAllowMove& operator=(DisableCopyAllowMove&&) = default;
	};

	struct ExplicitDefaultCtor
	{
		explicit ExplicitDefaultCtor() = default;
	};

}