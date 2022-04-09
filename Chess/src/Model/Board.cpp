// Author:	Liam Scholte
// Created:	3/26/2022 9:50:26 PM
// This file contains the implementations for Board
// See Board.h for documentation

#include <Chess/Model/Board.h>
#include <Chess/Model/Pawn.h>
#include <Chess/Model/Rook.h>
#include <Chess/Model/Knight.h>
#include <Chess/Model/Bishop.h>
#include <Chess/Model/Queen.h>
#include <Chess/Model/King.h>
#include <Chess/Model/Position.h>
#include <Chess/Model/Size.h>

#include <memory>
#include <unordered_set>

namespace Chess
{
namespace Model
{
	struct Board::Impl
	{
		std::unordered_set<std::shared_ptr<Piece>> pieces;
		std::unordered_set<std::shared_ptr<Piece>> whitePieces;
		std::unordered_set<std::shared_ptr<Piece>> blackPieces;

		std::shared_ptr<Piece> pWhiteKing;
		std::shared_ptr<Piece> pBlackKing;

		Impl()
		{			
			//White
			whitePieces.insert(std::make_shared<Rook>(true,	Position(1, 1)));
			whitePieces.insert(std::make_shared<Knight>(true, Position(1, 2)));
			whitePieces.insert(std::make_shared<Bishop>(true, Position(1, 3)));
			whitePieces.insert(std::make_shared<Queen>(true,	Position(1, 4)));
			pWhiteKing = *whitePieces.insert(std::make_shared<King>(true,	Position(1, 5))).first;
			whitePieces.insert(std::make_shared<Bishop>(true, Position(1, 6)));
			whitePieces.insert(std::make_shared<Knight>(true, Position(1, 7)));
			whitePieces.insert(std::make_shared<Rook>(true,	Position(1, 8)));

			whitePieces.insert(std::make_shared<Pawn>(true, Position(2, 1)));
			whitePieces.insert(std::make_shared<Pawn>(true, Position(2, 2)));
			whitePieces.insert(std::make_shared<Pawn>(true, Position(2, 3)));
			whitePieces.insert(std::make_shared<Pawn>(true, Position(2, 4)));
			whitePieces.insert(std::make_shared<Pawn>(true, Position(2, 5)));
			whitePieces.insert(std::make_shared<Pawn>(true, Position(2, 6)));
			whitePieces.insert(std::make_shared<Pawn>(true, Position(2, 7)));
			whitePieces.insert(std::make_shared<Pawn>(true, Position(2, 8)));

			//Black
			blackPieces.insert(std::make_shared<Rook>(false,		Position(8, 1)));
			blackPieces.insert(std::make_shared<Knight>(false,	Position(8, 2)));
			blackPieces.insert(std::make_shared<Bishop>(false,	Position(8, 3)));
			blackPieces.insert(std::make_shared<Queen>(false,		Position(8, 4)));
			pBlackKing = *blackPieces.insert(std::make_shared<King>(false,		Position(8, 5))).first;
			blackPieces.insert(std::make_shared<Bishop>(false,	Position(8, 6)));
			blackPieces.insert(std::make_shared<Knight>(false,	Position(8, 7)));
			blackPieces.insert(std::make_shared<Rook>(false,		Position(8, 8)));

			blackPieces.insert(std::make_shared<Pawn>(false, Position(7, 1)));
			blackPieces.insert(std::make_shared<Pawn>(false, Position(7, 2)));
			blackPieces.insert(std::make_shared<Pawn>(false, Position(7, 3)));
			blackPieces.insert(std::make_shared<Pawn>(false, Position(7, 4)));
			blackPieces.insert(std::make_shared<Pawn>(false, Position(7, 5)));
			blackPieces.insert(std::make_shared<Pawn>(false, Position(7, 6)));
			blackPieces.insert(std::make_shared<Pawn>(false, Position(7, 7)));
			blackPieces.insert(std::make_shared<Pawn>(false, Position(7, 8)));

			pieces.insert(whitePieces.cbegin(), whitePieces.cend());
			pieces.insert(blackPieces.cbegin(), blackPieces.cend());
		}

		Impl(Impl const& otherImpl)
		{
			for (std::shared_ptr<Piece> const& pPiece : otherImpl.whitePieces)
			{
				std::shared_ptr<Piece> pCopiedPiece = *whitePieces.insert(pPiece->clone()).first;
				if (pCopiedPiece->getType() == PieceType::King)
				{
					pWhiteKing = pCopiedPiece;
				}
			}

			for (std::shared_ptr<Piece> const& pPiece : otherImpl.blackPieces)
			{
				std::shared_ptr<Piece> pCopiedPiece = *blackPieces.insert(pPiece->clone()).first;
				if (pCopiedPiece->getType() == PieceType::King)
				{
					pBlackKing = pCopiedPiece;
				}
			}

			pieces.insert(whitePieces.cbegin(), whitePieces.cend());
			pieces.insert(blackPieces.cbegin(), blackPieces.cend());
		}
	};

	Board::Board()
		: m_pImpl(std::make_shared<Impl>())
	{}

	Board::Board(Board const& otherBoard)
		: m_pImpl(std::make_shared<Impl>(*otherBoard.m_pImpl))
	{
	}

	Board::~Board() = default;

	Size Board::getSize() const
	{
		return Size(8, 8);
	}

	std::unordered_set<std::shared_ptr<Piece>> const& Board::getPieces() const
	{
		return m_pImpl->pieces;
	}

	std::unordered_set<std::shared_ptr<Piece>> const& Board::getPieces(bool isWhite) const
	{
		return isWhite ? m_pImpl->whitePieces : m_pImpl->blackPieces;
	}

	std::shared_ptr<Piece> Board::getPiece(Position position) const
	{
		auto iter = std::find_if(
			m_pImpl->pieces.cbegin(),
			m_pImpl->pieces.cend(),
			[position](std::shared_ptr<Piece> pPiece)
			{
				return pPiece->getPosition() == position;
			});

		return iter == m_pImpl->pieces.cend()
			? nullptr
			: *iter;
	}

	bool Board::removePiece(std::shared_ptr<Piece> pPiece)
	{
		if (pPiece->isWhite())
		{
			m_pImpl->whitePieces.erase(pPiece);
		}
		else
		{
			m_pImpl->blackPieces.erase(pPiece);
		}
		return m_pImpl->pieces.erase(pPiece) > 0;
	}

	std::shared_ptr<Piece> Board::promotePiece(std::shared_ptr<Piece> pPieceToPromote, PieceType promotionType)
	{
		return nullptr;
	}

	bool Board::isPositionOnBoard(Position position) const
	{
		if (position.rank < 1 ||
			position.file < 1 ||
			position.rank > 8 ||
			position.file > 8)
		{
			return false;
		}
		return true;
	}

	bool Board::isPositionPossible(bool isWhite, Position position) const
	{
		if (!isPositionOnBoard(position))
		{
			//Position is outside chessboard boundary
			return false;
		}

		auto iter = std::find_if(
			m_pImpl->pieces.cbegin(),
			m_pImpl->pieces.cend(),
			[isWhite, position](std::shared_ptr<Piece> pPiece)
			{
				return pPiece->isWhite() == isWhite && pPiece->getPosition() == position;
			});
		if (iter != m_pImpl->pieces.cend())
		{
			//A piece of the same color exists in the position
			return false;
		}

		return true;
	}

	bool Board::isKingInCheck(bool isWhite) const
	{
		return isWhite
			? m_pImpl->pWhiteKing->isUnderAttack(*this)
			: m_pImpl->pBlackKing->isUnderAttack(*this);
	}


}
}
