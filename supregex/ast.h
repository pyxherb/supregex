#ifndef _SUPREGEX_AST_H_
#define _SUPREGEX_AST_H_

#include "basedefs.h"

#include <cstdint>

#include <peff/containers/dynarray.h>
#include <peff/containers/set.h>
#include <peff/containers/bitarray.h>
#include <variant>

namespace supregex {
	enum class AstNodeType {
		Chars = 0,
		UChars,

		CharSet,
		UCharSet,

		BeginningAnchor, // ^
		EndAnchor, // $

		WordBoundary, // \b
		NonWordBoundary, // \B

		ReservedChars, // \+

		CapturingGroup,
		NamedCapturingGroup,
		NumericRef,
		NonCapturingGroup,

		PosLookahead,
		NegLookahead,
		PosLookbehind,
		NegLookbehind,

		Alternation,
	};

	enum class QualifierType {
		None = 0,
		Plus,
		Star,
		Quantifier,

		Optional,
	};

	struct Qualifier {
		struct {
			struct {
				size_t minTimes;
				size_t maxTimes;
			} asQuantifier;
		} data;
		QualifierType qualifierType;
		bool isLazy;
	};

	class AstNode {
	public:
		peff::RcObjectPtr<peff::Alloc> selfAllocator;
		const AstNodeType astNodeType;
		Qualifier qualifier;

		SUPREGEX_API AstNode(AstNodeType astNodeType, peff::Alloc *selfAllocator);
	};

	class CharsAstNode : public AstNode {
	public:
		peff::DynArray<char> data;
	};

	enum CharSetDataStorageForm : uint8_t {
		CharSet = 0,
		BitMap
	};

	class CharSet : public AstNode {
	public:
		std::variant<peff::Set<char>, peff::BitArray> data;
		CharSetDataStorageForm dataStorageForm;
	};
}

#endif
