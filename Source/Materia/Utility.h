#pragma once

#include "CoreMinimal.h"

#include "libPNG-1.5.2/png.h"

#include "Networking.h"
#include "Json.h"

class MATERIA_API Utility {

public:

    static bool Send(FSocket* Socket, TSharedRef<FJsonObject> SentObj);
    static bool Recv(FSocket* Socket, TSharedPtr<FJsonObject>& RecvObj);
    
    static bool TryGetObjectField(
            TSharedPtr<FJsonObject> Obj,
            const FString& Field,
            TSharedPtr<FJsonObject>& Res
            );

    static void CompressImageDataPng(uint32 ImageWidth, uint32 ImageHeight, const TArray<FColor>& ImageData, TArray<uint8>& Output);
	static void CompressImageDataJpg(uint32 ImageWidth, uint32 ImageHeight, const TArray<FColor>& ImageData, TArray<uint8>& Output);

private:

    struct PngDestructor {
        png_struct* p;
        PngDestructor(png_struct* p) : p(p) {}
        ~PngDestructor() { if (p) { png_destroy_write_struct(&p, NULL); } }
    };

    static void PngWriteCallback(png_structp png_ptr, png_bytep data, png_size_t length);

};
