
#include "bz2lib.h"





int initcompress(bz_stream *stream, bz_struct *bzStruct) {
	memset(stream, 0, sizeof(bz_stream));
	memset(bzStruct, 0, sizeof(bz_struct));

	stream->bzalloc = NULL;
	stream->bzfree = NULL;
	stream->opaque = NULL;

	stream->avail_out = BUFFER_SIZE;
	stream->next_out = bzStruct->outputBuffer;

	if (BZ2_bzCompressInit(stream, BLOCK_SIZE, VERBOSITY, WORKFACTOR) != BZ_OK) {
		return COMP_INIT_ERROR;
	}

	return OK;
}

int compressAndWrite(char *sourceFile, char *destFile, bz_stream *stream,
		bz_struct *bzStruct) {
	int byteRead;
	int compressRet;
	int byteWrote;

	bzStruct->sourceFileDesc = open(sourceFile, O_RDONLY);
	bzStruct->destFileDesc = open(destFile, O_WRONLY | O_CREAT);

	if (bzStruct->sourceFileDesc == -1) {
		return FILE_OPEN_ERROR;
	}

	if (bzStruct->destFileDesc == -1) {
		return FILE_OPEN_ERROR;
	}
	stream->avail_out = BUFFER_SIZE;
	stream->next_out = bzStruct->outputBuffer;
	do {
		if (stream->avail_in == 0) {
			byteRead = read(bzStruct->sourceFileDesc, bzStruct->inputBuffer,
			BUFFER_SIZE);
			if (byteRead == -1) {
				return FILE_READ_ERROR;
			}

			stream->avail_in = byteRead;
			stream->next_in = bzStruct->inputBuffer;

			if (byteRead < BUFFER_SIZE) {
				break;
			}
		}

		compressRet = BZ2_bzCompress(stream, BZ_RUN);
		if (compressRet != BZ_RUN_OK) {
			return COMPRESS_ERROR;
		}

		if (stream->avail_out != BUFFER_SIZE) {
			byteWrote = write(bzStruct->destFileDesc, bzStruct->outputBuffer,
			BUFFER_SIZE - stream->avail_out);
			if (byteWrote == -1) {
				return FILE_WRITE_ERROR;
			}
			stream->avail_out = BUFFER_SIZE;
			stream->next_out = bzStruct->outputBuffer;
		}
	} while (true);

	do {
		compressRet = BZ2_bzCompress(stream, BZ_FINISH);
		if (compressRet == BZ_FINISH_OK || compressRet == BZ_STREAM_END) {
			byteWrote = write(bzStruct->destFileDesc, bzStruct->outputBuffer,
			BUFFER_SIZE - stream->avail_out);
			if (byteWrote == -1) {
				return FILE_WRITE_ERROR;
			}
			stream->avail_out = BUFFER_SIZE;
			stream->next_out = bzStruct->outputBuffer;
		} else
			return COMPRESS_ERROR;
	} while (compressRet != BZ_STREAM_END);
	return OK;
}

int finilizeCompression(bz_stream *stream, bz_struct *bzStruct) {
	if (BZ2_bzCompressEnd(stream) != BZ_OK) {
		return COMPRESS_END_ERROR;
	}

	if (close(bzStruct->sourceFileDesc) == -1) {
		return FILE_CLOSE_ERROR;
	}

	if (close(bzStruct->destFileDesc) == -1) {
		return FILE_CLOSE_ERROR;
	}

	return OK;
}

int compress(gchar *source, gchar *dest) {
	bz_struct *bzStruct;
	bz_stream *stream;

	stream = malloc(sizeof(bz_stream));
	bzStruct = malloc(sizeof(bz_struct));

	if (initcompress(stream, bzStruct) != OK) {

		printf("initcompress  not successful...\n");
		return 0;
	}

	if (compressAndWrite(source, dest, stream, bzStruct) != OK) {
		free(bzStruct);
		free(stream);
		return 0;
	}

	if (finilizeCompression(stream, bzStruct) != OK) {
		free(bzStruct);
		free(stream);
		return 0;
	}
	free(bzStruct);
	free(stream);
	return 1;
}

int decompress(gchar *source, gchar *dest) {

	bz_struct *bzStruct;
	bz_stream *stream;

	stream = malloc(sizeof(bz_stream));
	bzStruct = malloc(sizeof(bz_struct));

	if (initializeDecompress(stream, bzStruct) != OK) {
		printf("initdecompress  not successful...\n");
		return 0;
	}

	if (DecompressAndWrite(source, dest, stream, bzStruct) != OK) {
		free(bzStruct);
		free(stream);
		return 0;
	}

	if (decompressEnd(stream, bzStruct) != OK) {
		free(bzStruct);
		free(stream);
		return 0;
	}
	free(bzStruct);
	free(stream);
	return 1;
}

int initializeDecompress(bz_stream *stream, bz_struct *bzStruct)
{
	memset(stream, 0, sizeof(bz_stream));
	memset(bzStruct, 0, sizeof(bz_struct));

	stream->bzalloc = NULL;
	stream->bzfree = NULL;
	stream->opaque = NULL;

	stream->avail_out = BUFFER_SIZE;
	stream->next_out = bzStruct->outputBuffer;

	if (BZ2_bzDecompressInit(stream, VERBOSITY, SMALL) != BZ_OK)
	{
		return INIT_DECOMPRESS_ERROR;
	}

	return OK;
}

int DecompressAndWrite(char *sourceFile, char *destFile, bz_stream *stream,
		bz_struct *bzStruct)
{
	int byteRead;
	int totalRead = 0;
	int byteWrote;
	int decompressRet;
	int totalWrote = 0;

	bzStruct->sourceFileDesc = open(sourceFile, O_RDONLY);
	bzStruct->destFileDesc = open(destFile, O_WRONLY | O_CREAT);

	if (bzStruct->sourceFileDesc == -1)
	{
		return FILE_OPEN_ERROR;
	}

	if (bzStruct->destFileDesc == -1)
	{
		return FILE_OPEN_ERROR;
	}

	do
	{
		byteRead = read(bzStruct->sourceFileDesc, bzStruct->inputBuffer, BUFFER_SIZE);

		if (byteRead == -1)
		{
			return FILE_READ_ERROR;
		}

		totalRead += byteRead;
		stream->avail_in = byteRead;
		stream->next_in = bzStruct->inputBuffer;
		if(byteRead==0)
		{
			return OK;
		}
		decompressRet = BZ2_bzDecompress(stream);

		if (decompressRet != BZ_OK && decompressRet != BZ_STREAM_END)
		{
			return DECOMPRESS_ERROR;
		}
		if (stream->avail_out != BUFFER_SIZE)
		{
			do
			{
				byteWrote = write(bzStruct->destFileDesc, bzStruct->outputBuffer,
				BUFFER_SIZE - stream->avail_out);

				if (byteWrote == -1)
				{
					return FILE_WRITE_ERROR;
				}

				stream->next_out = bzStruct->outputBuffer;
				stream->avail_out = BUFFER_SIZE;
				totalWrote += byteWrote;

				decompressRet = BZ2_bzDecompress(stream);
				if (decompressRet != BZ_OK && decompressRet != BZ_STREAM_END)
				{
					return DECOMPRESS_ERROR;
				}
			}while(decompressRet != BZ_STREAM_END && stream->avail_out != BUFFER_SIZE);

			byteWrote = write(bzStruct->destFileDesc, bzStruct->outputBuffer,
			BUFFER_SIZE - stream->avail_out);

			if (byteWrote == -1)
			{
				return FILE_WRITE_ERROR;
			}
		}

	} while (byteRead != 0);


	return OK;
}

int decompressEnd(bz_stream *stream, bz_struct *bzStruct)
{
	if (BZ2_bzDecompressEnd(stream) != BZ_OK)
	{
		return DECOM_END_ERROR;
	}

	if (close(bzStruct->sourceFileDesc) == -1)
	{
		return FILE_CLOSE_ERROR;
	}

	if (close(bzStruct->destFileDesc) == -1)
	{
		return FILE_CLOSE_ERROR;
	}

	return OK;
}
 
