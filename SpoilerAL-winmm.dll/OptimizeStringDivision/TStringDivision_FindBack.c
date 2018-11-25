#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

#define ESCAPE_TAG          '\\'
#define MAX_NEST_TAG_LENGTH 2

unsigned long __cdecl TStringDivision_FindBack(
	IN     TStringDivision *this,
	IN     const string    *Src,
	IN     string          Token,
	IN     unsigned long   FromIndex,
	IN     unsigned long   ToIndex,
	IN     unsigned long   Option)
{
	size_t TokenLength;
	size_t SrcLength;
	LPCSTR SrcIt, SrcEnd;
	size_t NestStartTagLength;
	size_t NestEndTagLength;
	size_t FindIndex;

	if (ToIndex == FromIndex)
		goto FAILED;

	TokenLength = string_length(&Token);
	SrcLength = string_length(Src);

	// ��������r���̕����Z���Ȃ�Ę_�O(^^;
	if (SrcLength < TokenLength)
		goto FAILED;

	// �����I�[������������Ε␳
	if (SrcLength < FromIndex || SrcLength < FromIndex + TokenLength)
		FromIndex = SrcLength - TokenLength + 1;

	SrcIt = Src->_M_start + ToIndex;
	SrcEnd = Src->_M_start + FromIndex;

	if (Option & dtNEST)
	{
		NestStartTagLength = string_length(&this->nestStartTag);
		NestEndTagLength = string_length(&this->nestEndTag);
		if (NestStartTagLength == 0 || NestEndTagLength == 0)
			goto FAILED;
	}

	// �����̃p�^�[���}�b�`���O�Ȃ�Boyer-Moore�@���Ď�����邪�A
	// 2�o�C�g������l�X�g�A�G�X�P�[�v�V�[�P���X�������Ă���̂�1�Â�(^^;)

	FindIndex = (unsigned long)SIZE_MAX;
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
					if (SrcIt[0] == this->nestStartTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == this->nestStartTag._M_start[1]))
					{
						size_t NCount;

						// �l�X�g�J�n
						NCount = 1;
						SrcIt += NestStartTagLength;
						while (SrcIt < SrcEnd)
						{
							if (*SrcIt != ESCAPE_TAG)
							{
								if (SrcIt[0] == this->nestStartTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == this->nestStartTag._M_start[1]))
								{
									// ����Ƀl�X�g
									SrcIt += NestStartTagLength;
									NCount++;
									continue;
								}
								if (SrcIt[0] == this->nestEndTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == this->nestEndTag._M_start[1]))
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
					if (memcmp(SrcIt, Token._M_start, TokenLength) == 0)
						FindIndex = SrcIt - Src->_M_start;
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
				if (SrcIt[0] == this->nestStartTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == this->nestStartTag._M_start[1]))
				{
					size_t NCount;

					// �l�X�g�J�n
					NCount = 1;
					SrcIt += NestStartTagLength;
					while (SrcIt < SrcEnd)
					{
						if (SrcIt[0] == this->nestStartTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == this->nestStartTag._M_start[1]))
						{
							// ����Ƀl�X�g
							SrcIt += NestStartTagLength;
							NCount++;
							continue;
						}
						if (SrcIt[0] == this->nestEndTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == this->nestEndTag._M_start[1]))
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
				if (memcmp(SrcIt, Token._M_start, TokenLength) == 0)
					FindIndex = SrcIt - Src->_M_start;

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
				if (memcmp(SrcIt, Token._M_start, TokenLength) == 0)
					FindIndex = SrcIt - Src->_M_start;
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
			if (memcmp(SrcIt, Token._M_start, TokenLength) == 0)
				FindIndex = SrcIt - Src->_M_start;

			if (!__intrinsic_isleadbyte(*SrcIt))
				SrcIt++;
			else
				SrcIt += 2;
		}
	}

	string_dtor(&Token);
	return (unsigned long)FindIndex;

FAILED:
	string_dtor(&Token);
	return (unsigned long)SIZE_MAX;
}

