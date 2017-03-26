#include <windows.h>
#include "intrinsic.h"
#include "TStringDivision.h"

#pragma function(memcmp)

unsigned long __cdecl TStringDivision_FindBack(
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option)
{
	size_t TokenLength;
	size_t SrcLength;
	LPCSTR SrcIt, SrcEnd;
	size_t NestStartTagLength;
	size_t NestEndTagLength;
	size_t EscapeTagLength;
	size_t FindIndex;

	if (ToIndex == FromIndex)
		goto FAILED;

	TokenLength = bcb6_std_string_length(&Token);
	SrcLength = bcb6_std_string_length(Src);

	// ��������r���̕����Z���Ȃ�Ę_�O(^^;
	if (SrcLength < TokenLength)
		goto FAILED;

	// �����I�[������������Ε␳
	if (SrcLength < FromIndex || SrcLength < FromIndex + TokenLength)
		FromIndex = SrcLength - TokenLength + 1;

	SrcIt = Src->_M_start + ToIndex;
	SrcEnd = Src->_M_start + FromIndex;

	if (Option & DT_NEST)
	{
		NestStartTagLength = bcb6_std_string_length(&_this->nestStartTag);
		NestEndTagLength = bcb6_std_string_length(&_this->nestEndTag);
		if (NestStartTagLength == 0 || NestEndTagLength == 0)
			goto FAILED;
	}
	if (Option & DT_ESCAPE)
	{
		EscapeTagLength = bcb6_std_string_length(&_this->escapeTag);
		if (EscapeTagLength == 0)
			goto FAILED;
	}

	// �����̃p�^�[���}�b�`���O�Ȃ�Boyer-Moore�@���Ď�����邪�A
	// 2�o�C�g������l�X�g�A�G�X�P�[�v�V�[�P���X�������Ă���̂�1�Â�(^^;)

	FindIndex = (unsigned long)SIZE_MAX;
	if (Option & DT_NEST)
	{
		// �l�X�g�`�F�b�N����
		if (Option & DT_ESCAPE)
		{
			// �G�X�P�[�v�V�[�P���X�g�p
			while (SrcIt < SrcEnd)
			{
				if (memcmp(SrcIt, _this->escapeTag._M_start, EscapeTagLength) != 0)
				{
					if (memcmp(SrcIt, _this->nestStartTag._M_start, NestStartTagLength) == 0)
					{
						size_t NCount;

						// �l�X�g�J�n
						NCount = 1;
						SrcIt += NestStartTagLength;
						while (SrcIt < SrcEnd)
						{
							if (memcmp(SrcIt, _this->escapeTag._M_start, EscapeTagLength) != 0)
							{
								if (memcmp(SrcIt, _this->nestStartTag._M_start, NestStartTagLength) == 0)
								{
									// ����Ƀl�X�g
									SrcIt += NestStartTagLength;
									NCount++;
									continue;
								}
								if (memcmp(SrcIt, _this->nestEndTag._M_start, NestEndTagLength) == 0)
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
								SrcIt += EscapeTagLength;
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
					SrcIt += EscapeTagLength;
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
				if (memcmp(SrcIt, _this->nestStartTag._M_start, NestStartTagLength) == 0)
				{
					size_t NCount;

					// �l�X�g�J�n
					NCount = 1;
					SrcIt += NestStartTagLength;
					while (SrcIt < SrcEnd)
					{
						if (memcmp(SrcIt, _this->nestStartTag._M_start, NestStartTagLength) == 0)
						{
							// ����Ƀl�X�g
							SrcIt += NestStartTagLength;
							NCount++;
							continue;
						}
						if (memcmp(SrcIt, _this->nestEndTag._M_start, NestEndTagLength) == 0)
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
	else if (Option & DT_ESCAPE)
	{
		// �G�X�P�[�v�V�[�P���X�g�p
		while (SrcIt < SrcEnd)
		{
			if (memcmp(SrcIt, _this->escapeTag._M_start, EscapeTagLength) != 0)
			{
				// ��{��r����
				if (memcmp(SrcIt, Token._M_start, TokenLength) == 0)
					FindIndex = SrcIt - Src->_M_start;
			}
			else
			{
				// �G�X�P�[�v�V�[�P���X�Ɉ���������܂���
				SrcIt += EscapeTagLength;
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

	bcb6_std_string_dtor(&Token);
	return (unsigned long)FindIndex;

FAILED:
	bcb6_std_string_dtor(&Token);
	return (unsigned long)SIZE_MAX;
}

