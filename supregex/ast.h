#ifndef _SUPREGEX_AST_H_
#define _SUPREGEX_AST_H_

#include "basedefs.h"

#include <cstdint>

#include <peff/base/deallocable.h>
#include <peff/containers/dynarray.h>
#include <peff/containers/string.h>
#include <peff/containers/set.h>
#include <peff/containers/bitarray.h>
#include <variant>
#include <atomic>

namespace supregex {
	enum class AstNodeType {
		Chars = 0,
		UChars,

		CharSet,
		UCharSet,

		BeginningAnchor,  // ^
		EndAnchor,		  // $

		WordBoundary,	  // \b
		NonWordBoundary,  // \B

		ReservedChars,	// \+

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
		QualifierType qualifierType = QualifierType::None;
		bool isLazy = false;
	};

	class AstNode;

	struct AstNodeDestructionInfo {
		AstNode *destructibleAstNodeList;

		SUPREGEX_API void pushDestructible(AstNode *astNode);
	};

	class AstNode {
	public:
		AstNodeDestructionInfo *destructionInfo;
		AstNode *nextDestructible = nullptr;

		peff::RcObjectPtr<peff::Alloc> selfAllocator;
		const AstNodeType astNodeType;
		Qualifier qualifier;

		SUPREGEX_API AstNode(AstNodeType astNodeType, peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~AstNode();

		virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept = 0;

		SUPREGEX_API void dealloc() noexcept;
	};

	class CharsNode : public AstNode {
	public:
		peff::RcObjectPtr<peff::Alloc> dataAllocator;
		char *const data;
		const size_t size;

		SUPREGEX_API CharsNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~CharsNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};

	enum CharSetDataStorageForm : uint8_t {
		Unset = 0,
		Set,
		BitMap
	};

	class CharSetNode : public AstNode {
	public:
		using SetStorage = peff::Set<char>;
		using SetStoragePtr = std::unique_ptr<SetStorage, peff::ParamBasedAllocUniquePtrDeleter<SetStorage>>;
		using BitMapStorage = peff::BitArray;
		using BitMapStoragePtr = std::unique_ptr<BitMapStorage, peff::ParamBasedAllocUniquePtrDeleter<BitMapStorage>>;

		std::variant<SetStoragePtr, BitMapStoragePtr> data;
		CharSetDataStorageForm dataStorageForm;

		SUPREGEX_API CharSetNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~CharSetNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};

	class BeginningAnchorNode final : public AstNode {
	public:
		SUPREGEX_API BeginningAnchorNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~BeginningAnchorNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};

	class EndAnchorNode final : public AstNode {
	public:
		SUPREGEX_API EndAnchorNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~EndAnchorNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};

	class WordBoundaryNode final : public AstNode {
	public:
		SUPREGEX_API WordBoundaryNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~WordBoundaryNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};

	class NonWordBoundaryNode final : public AstNode {
	public:
		SUPREGEX_API NonWordBoundaryNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~NonWordBoundaryNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};

	class ReservedCharsNode final : public AstNode {
	public:
		SUPREGEX_API ReservedCharsNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~ReservedCharsNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};

	class CapturingGroupNode final : public AstNode {
	public:
		AstNode *capturedExpr = nullptr;

		SUPREGEX_API CapturingGroupNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~CapturingGroupNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};

	class NamedCapturingGroupNode final : public AstNode {
	public:
		peff::String name;
		AstNode *capturedExpr = nullptr;

		SUPREGEX_API NamedCapturingGroupNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~NamedCapturingGroupNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};

	class NumericRefNode final : public AstNode {
	public:
		size_t index = SIZE_MAX;

		SUPREGEX_API NumericRefNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~NumericRefNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};

	class NonCapturingGroupNode final : public AstNode {
	public:
		AstNode *capturedExpr = nullptr;

		SUPREGEX_API NonCapturingGroupNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~NonCapturingGroupNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};

	class AlternationNode final : public AstNode {
	public:
		AstNode *lhs = nullptr;
		AstNode *rhs = nullptr;

		SUPREGEX_API AlternationNode(peff::Alloc *selfAllocator);
		SUPREGEX_API virtual ~AlternationNode();

		SUPREGEX_API virtual void dealloc(AstNodeDestructionInfo &destructionInfo) noexcept override;
	};
}

#endif
