#pragma once
#include <string>
#include "Ticket.h"
#include "Booking.h"
class QRCodeService {
public:
    static void GenerateTicketQR(Ticket& ticket) {
        ticket.GenerateQrCodeData();
    }
};
