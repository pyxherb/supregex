#include "ast.h"

using namespace supregex;

SUPREGEX_API void AstNodeDestructionInfo::pushDestructible(AstNode *astNode) {
	astNode->nextDestructible = destructibleAstNodeList;
	destructibleAstNodeList = astNode;
}

SUPREGEX_API AstNode::AstNode(AstNodeType astNodeType, peff::Alloc *selfAllocator) : astNodeType(astNodeType), selfAllocator(selfAllocator) {
}

SUPREGEX_API AstNode::~AstNode() {
}

SUPREGEX_API void AstNode::dealloc() noexcept {
	AstNodeDestructionInfo destructionInfo;

	assert(!this->destructionInfo);

	dealloc(destructionInfo);

	AstNode *next;

	while ((next = destructionInfo.destructibleAstNodeList)) {
		destructionInfo.destructibleAstNodeList = nullptr;
		do {
			next->dealloc(destructionInfo);
		} while ((next = next->nextDestructible));
	}
}

SUPREGEX_API CharsNode::CharsNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::Chars, selfAllocator), data(nullptr), size(0) {
}

SUPREGEX_API CharsNode::~CharsNode() {
	if (data) {
		dataAllocator->release(data, size, alignof(char));
	}
}

SUPREGEX_API void CharsNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
	peff::destroyAndRelease<CharsNode>(selfAllocator.get(), this, alignof(CharsNode));
}

SUPREGEX_API CharSetNode::CharSetNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::CharSet, selfAllocator), dataStorageForm(CharSetDataStorageForm::Unset) {
}

SUPREGEX_API CharSetNode::~CharSetNode() {
}

SUPREGEX_API void CharSetNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
	peff::destroyAndRelease<CharSetNode>(selfAllocator.get(), this, alignof(CharSetNode));
}

SUPREGEX_API BeginningAnchorNode::BeginningAnchorNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::BeginningAnchor, selfAllocator) {
}

SUPREGEX_API BeginningAnchorNode::~BeginningAnchorNode() {
}

SUPREGEX_API void BeginningAnchorNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
	peff::destroyAndRelease<BeginningAnchorNode>(selfAllocator.get(), this, alignof(BeginningAnchorNode));
}

SUPREGEX_API EndAnchorNode::EndAnchorNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::EndAnchor, selfAllocator) {
}

SUPREGEX_API EndAnchorNode::~EndAnchorNode() {
}

SUPREGEX_API void EndAnchorNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
	peff::destroyAndRelease<EndAnchorNode>(selfAllocator.get(), this, alignof(EndAnchorNode));
}

SUPREGEX_API WordBoundaryNode::WordBoundaryNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::WordBoundary, selfAllocator) {
}

SUPREGEX_API WordBoundaryNode::~WordBoundaryNode() {
}

SUPREGEX_API void WordBoundaryNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
	peff::destroyAndRelease<WordBoundaryNode>(selfAllocator.get(), this, alignof(WordBoundaryNode));
}

SUPREGEX_API NonWordBoundaryNode::NonWordBoundaryNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::NonWordBoundary, selfAllocator) {
}

SUPREGEX_API NonWordBoundaryNode::~NonWordBoundaryNode() {
}

SUPREGEX_API void NonWordBoundaryNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
	peff::destroyAndRelease<NonWordBoundaryNode>(selfAllocator.get(), this, alignof(NonWordBoundaryNode));
}

SUPREGEX_API ReservedCharsNode::ReservedCharsNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::ReservedChars, selfAllocator) {
}

SUPREGEX_API ReservedCharsNode::~ReservedCharsNode() {
}

SUPREGEX_API void ReservedCharsNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
	peff::destroyAndRelease<ReservedCharsNode>(selfAllocator.get(), this, alignof(ReservedCharsNode));
}

SUPREGEX_API CapturingGroupNode::CapturingGroupNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::CapturingGroup, selfAllocator) {
}

SUPREGEX_API CapturingGroupNode::~CapturingGroupNode() {
	if (capturedExpr) {
		destructionInfo->pushDestructible(capturedExpr);
	}
}

SUPREGEX_API void CapturingGroupNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
	peff::destroyAndRelease<CapturingGroupNode>(selfAllocator.get(), this, alignof(CapturingGroupNode));
}

SUPREGEX_API NamedCapturingGroupNode::NamedCapturingGroupNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::NamedCapturingGroup, selfAllocator), name(selfAllocator) {
}

SUPREGEX_API NamedCapturingGroupNode::~NamedCapturingGroupNode() {
	if (capturedExpr) {
		destructionInfo->pushDestructible(capturedExpr);
	}
}

SUPREGEX_API void NamedCapturingGroupNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
	this->destructionInfo = &destructionInfo;

	peff::destroyAndRelease<NamedCapturingGroupNode>(selfAllocator.get(), this, alignof(NamedCapturingGroupNode));
}

SUPREGEX_API NumericRefNode::NumericRefNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::WordBoundary, selfAllocator) {
}

SUPREGEX_API NumericRefNode::~NumericRefNode() {
}

SUPREGEX_API void NumericRefNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
	peff::destroyAndRelease<NumericRefNode>(selfAllocator.get(), this, alignof(NumericRefNode));
}

SUPREGEX_API NonCapturingGroupNode::NonCapturingGroupNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::NonCapturingGroup, selfAllocator) {
}

SUPREGEX_API NonCapturingGroupNode::~NonCapturingGroupNode() {
	if (capturedExpr) {
		destructionInfo->pushDestructible(capturedExpr);
	}
}

SUPREGEX_API void NonCapturingGroupNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
	peff::destroyAndRelease<NonCapturingGroupNode>(selfAllocator.get(), this, alignof(NonCapturingGroupNode));
}

SUPREGEX_API AlternationNode::AlternationNode(peff::Alloc *selfAllocator) : AstNode(AstNodeType::Alternation, selfAllocator) {
}

SUPREGEX_API AlternationNode::~AlternationNode() {
	if (lhs) {
		destructionInfo->pushDestructible(lhs);
	}

	if (rhs) {
		destructionInfo->pushDestructible(rhs);
	}
}

SUPREGEX_API void AlternationNode::dealloc(AstNodeDestructionInfo &destructionInfo) noexcept {
}
