#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

unsigned long __cdecl TStringDivision_Find(
	IN     TStringDivision *this,
	IN     const string    *Src,
	IN     string          Token,
	IN     unsigned long   FromIndex,
	IN     unsigned long   ToIndex,
	IN     unsigned long   Option)
{
	unsigned long Result;

	Result = TStringDivision_Find_WithoutTokenDtor(
		this,
		Src,
		string_c_str(&Token),
		string_length(&Token),
		FromIndex,
		ToIndex,
		Option);
	string_dtor(&Token);
	return Result;
}

#define ESCAPE_TAG          '\\'
#define MAX_NEST_TAG_LENGTH 2

unsigned long __stdcall TStringDivision_Find_WithoutTokenDtor(
	IN     TStringDivision *this,
	IN     const string    *Src,
	IN     const char      *Token,
	IN     size_t          TokenLength,
	IN     unsigned long   FromIndex,
	IN     unsigned long   ToIndex,
	IN     unsigned long   Option)
{
	size_t SrcLength;
	LPCSTR SrcIt, SrcEnd;
	size_t NestStartTagLength;
	size_t NestEndTagLength;

	if (FromIndex == ToIndex)
		goto FAILED;

	SrcLength = string_length(Src);

	// ��������r���̕����Z���Ȃ�Ę_�O(^^;
	if (SrcLength < TokenLength)
		goto FAILED;

	// �����I�[������������Ε␳
	if (SrcLength < ToIndex || SrcLength < ToIndex + TokenLength)
		ToIndex = SrcLength - TokenLength + 1;

	SrcIt = string_c_str(Src) + FromIndex;
	SrcEnd = string_c_str(Src) + ToIndex;

	if (Option & dtNEST)
	{
		NestStartTagLength = string_length(&this->nestStartTag);
		NestEndTagLength = string_length(&this->nestEndTag);
		if (NestStartTagLength == 0 || NestEndTagLength == 0)
			goto FAILED;
	}

	// �����̃p�^�[���}�b�`���O�Ȃ�Boyer-Moore�@���Ď�����邪�A
	// 2�o�C�g������l�X�g�A�G�X�P�[�v�V�[�P���X�������Ă���̂�1�Â�(^^;)

	if (Option & dtNEST)
	{
		// �l�X�g�`�F�b�N����
		if (Option & dtESCAPE)
		{
			// �G�X�P�[�v�V�[�P���X�g�p
			while (SrcIt < SrcEnd)
			{
				if (*SrcIt != ESCAPE_TAG)
				{
					if (SrcIt[0] == string_at(&this->nestStartTag, 0) && (NestStartTagLength <= 1 || SrcIt[1] == string_at(&this->nestStartTag, 1)))
					{
						size_t NCount;

						// �l�X�g�J�n
						NCount = 1;
						SrcIt += NestStartTagLength;
						while (SrcIt < SrcEnd)
						{
							if (*SrcIt != ESCAPE_TAG)
							{
								if (SrcIt[0] == string_at(&this->nestStartTag, 0) && (NestStartTagLength <= 1 || SrcIt[1] == string_at(&this->nestStartTag, 1)))
								{
									// ����Ƀl�X�g
									SrcIt += NestStartTagLength;
									NCount++;
									continue;
								}
								if (SrcIt[0] == string_at(&this->nestEndTag, 0) && (NestEndTagLength <= 1 || SrcIt[1] == string_at(&this->nestEndTag, 1)))
								{
									// �l�X�g(��i)����
									SrcIt += NestEndTagLength;
									if (--NCount == 0)
										break;	// �l�X�g���S�E�o
									continue;
								}
							}
							else
							{
								SrcIt++;
							}
							if (!__intrinsic_isleadbyte(*SrcIt))
								SrcIt++;
							else
								SrcIt += 2;
						}
						continue;	// ����ɂ܂��l�X�g�J�n�����肦��ł́B
					}

					// ��{��r����
					if (memcmp(SrcIt, Token, TokenLength) == 0)
						goto TOKEN_FOUND;
				}
				else
				{
					// �G�X�P�[�v�V�[�P���X�Ɉ���������܂���
					SrcIt++;
				}

				if (!__intrinsic_isleadbyte(*SrcIt))
					SrcIt++;
				else
					SrcIt += 2;
			}
		}
		else
		{
			while (SrcIt < SrcEnd)
			{
				if (SrcIt[0] == string_at(&this->nestStartTag, 0) && (NestStartTagLength <= 1 || SrcIt[1] == string_at(&this->nestStartTag, 1)))
				{
					size_t NCount;

					// �l�X�g�J�n
					NCount = 1;
					SrcIt += NestStartTagLength;
					while (SrcIt < SrcEnd)
					{
						if (SrcIt[0] == string_at(&this->nestStartTag, 0) && (NestStartTagLength <= 1 || SrcIt[1] == string_at(&this->nestStartTag, 1)))
						{
							// ����Ƀl�X�g
							SrcIt += NestStartTagLength;
							NCount++;
							continue;
						}
						if (SrcIt[0] == string_at(&this->nestEndTag, 0) && (NestEndTagLength <= 1 || SrcIt[1] == string_at(&this->nestEndTag, 1)))
						{
							// �l�X�g(��i)����
							SrcIt += NestEndTagLength;
							if (--NCount == 0)
								break;	// �l�X�g���S�E�o
							continue;
						}
						if (!__intrinsic_isleadbyte(*SrcIt))
							SrcIt++;
						else
							SrcIt += 2;
					}
					continue;	// ����ɂ܂��l�X�g�J�n�����肦��ł́B
				}

				// ��{��r����
				if (memcmp(SrcIt, Token, TokenLength) == 0)
					goto TOKEN_FOUND;

				if (!__intrinsic_isleadbyte(*SrcIt))
					SrcIt++;
				else
					SrcIt += 2;
			}
		}
	}
	else if (Option & dtESCAPE)
	{
		// �G�X�P�[�v�V�[�P���X�g�p
		while (SrcIt < SrcEnd)
		{
			if (*SrcIt != ESCAPE_TAG)
			{
				// ��{��r����
				if (memcmp(SrcIt, Token, TokenLength) == 0)
					goto TOKEN_FOUND;
			}
			else
			{
				// �G�X�P�[�v�V�[�P���X�Ɉ���������܂���
				SrcIt++;
			}
			if (!__intrinsic_isleadbyte(*SrcIt))
				SrcIt++;
			else
				SrcIt += 2;
		}
	}
	else
	{
		// �P������
		while (SrcIt < SrcEnd)
		{
			// ��{��r����
			if (memcmp(SrcIt, Token, TokenLength) == 0)
				goto TOKEN_FOUND;

			if (!__intrinsic_isleadbyte(*SrcIt))
				SrcIt++;
			else
				SrcIt += 2;
		}
	}

FAILED:
	return (unsigned long)-1;

TOKEN_FOUND:
	return (unsigned long)(SrcIt - string_c_str(Src));
}

