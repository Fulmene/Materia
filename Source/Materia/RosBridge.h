// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Networking.h"
#include "Json.h"

/**
 * 
 */
class MATERIA_API RosBridge {

    public:
        static const FString DefaultIpAddr;
        static const uint16 DefaultPort;
        static const int32 BufferSize;

    public:
        RosBridge();
        RosBridge(const FString& IpAddr, uint16 Port);
        RosBridge(const FString& IpAddr, uint16 Port, bool IsUdp);

        // Uniqueness; Disallow copying
        RosBridge(const RosBridge&) = delete;
        RosBridge& operator=(const RosBridge&) = delete;
        RosBridge(RosBridge&&) = default;
        RosBridge& operator=(RosBridge&&) = default;

    public:
        ~RosBridge();

    public:

        bool Advertise(
                const FString& Topic,
                const FString& Type
                ) const;
        bool Unadvertise(
                const FString& Topic
                ) const;
        bool Publish(
                const FString& Topic,
                TSharedPtr<FJsonObject> Msg
                ) const;

		bool Subscribe(
				const FString& Topic,
				int32 ThrottleRate
		) const;
		bool Unsubscribe(
			const FString& Topic
		) const;

        bool CallService(
                const FString& Service
                ) const;
        bool CallService(
                const FString& Service,
                const TSharedPtr<FJsonObject>& Args
                ) const;
        bool CallService(
                const FString& Service,
                TSharedPtr<FJsonObject>& Values
                ) const;
        bool CallService(
                const FString& Service,
                const TSharedPtr<FJsonObject>& Args,
                TSharedPtr<FJsonObject>& Values
                ) const;

    private:
        bool Send(TSharedRef<FJsonObject> SentObj) const;
        bool Recv(TSharedPtr<FJsonObject>& RecvObj) const;

    private:
        FSocket* Socket;

};
