#include "Utility.h"
#include "ImageWrapper.h"

static IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

bool Utility::Send(FSocket* Socket, TSharedRef<FJsonObject> SentObj) {
    if (Socket == nullptr) return false;

    // Serialize JSON object to string
    FString SentString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&SentString);
    if (!FJsonSerializer::Serialize(SentObj, Writer)) return false;

    // Convert string to uint8 to send over socket
    int32 SentSize = 0;
    return Socket->Send(reinterpret_cast<const uint8*>(TCHAR_TO_ANSI(*SentString)), SentString.Len(), SentSize);

}

bool Utility::Recv(FSocket* Socket, TSharedPtr<FJsonObject>& RecvObj) {
    if (Socket == nullptr) return false;

    // Read uint8 byte stream from socket
    TArray<uint8> RecvStream;
    uint32 PendingSize;
    int32 RecvSize = 0;
    bool Received = false;
    while (Socket->HasPendingData(PendingSize)) {
        RecvStream.Init(0, FMath::Min(PendingSize, 65507u));
        Received = Socket->Recv(RecvStream.GetData(), PendingSize, RecvSize);
    }

    // Receive nothing
    if (!Received || RecvSize <= 0) return false;

    // Convert byte stream to string
    RecvStream.Add(0);
    FString RecvString( ANSI_TO_TCHAR( reinterpret_cast<ANSICHAR*>(RecvStream.GetData()) ) );
    // Deserialize the string into JSON object
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(RecvString);
    return FJsonSerializer::Deserialize(Reader, RecvObj);
}

bool Utility::TryGetObjectField(
        TSharedPtr<FJsonObject> Obj,
        const FString& Field,
        TSharedPtr<FJsonObject>& Res
        ) {
    bool Valid = Obj->HasTypedField<EJson::Object>(Field);
    if (Valid) Res = Obj->GetObjectField(Field);
    return Valid;
}

// Can't use ImageWrapper because ROS doesn't accept 4-channel (like RGBA) PNG
void Utility::CompressImageDataPng(uint32 ImageWidth, uint32 ImageHeight, const TArray<FColor>& ImageData, TArray<uint8>& Output) {
    Output.Empty();
    png_structp p = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    PngDestructor destroyPng(p);
    png_infop info_ptr = png_create_info_struct(p);

    png_set_IHDR(p, info_ptr, ImageWidth, ImageHeight, 8,
        PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    TArray<uint8> ImageStream;
    for (FColor c : ImageData) {
        ImageStream.Add(c.R);
        ImageStream.Add(c.G);
        ImageStream.Add(c.B);
    }
    TArray<uint8*> rows;
    for (uint32 y = 0; y < ImageHeight; y++)
        rows.Add(ImageStream.GetData() + y * ImageWidth * 3);
    png_set_rows(p, info_ptr, rows.GetData());
    png_set_write_fn(p, &Output, PngWriteCallback, NULL);
    png_write_png(p, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
}

void Utility::PngWriteCallback(png_structp png_ptr, png_bytep data, png_size_t length) {
    TArray<uint8>* p = (TArray<uint8>*)png_get_io_ptr(png_ptr);
    p->Append(data, length);
}

void Utility::CompressImageDataJpg(uint32 ImageWidth, uint32 ImageHeight, const TArray<FColor>& ImageData, TArray<uint8>& Output) {
	
	TArray<uint8> ImageStream;
	for (FColor c : ImageData) {
		ImageStream.Add(c.R);
		ImageStream.Add(c.G);
		ImageStream.Add(c.B);
		ImageStream.Add(255);
	}

	IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	ImageWrapper->SetRaw(ImageStream.GetData(), ImageStream.Num(), ImageWidth, ImageHeight, ERGBFormat::RGBA, 8);
	Output = ImageWrapper->GetCompressed();

}
