#pragma once
#include "Booking.h"
#include "DatabaseManager.h"
#include <string>
class BookingRepository {
public:
    BookingRepository() {}
    ~BookingRepository() {}
    bool CreateBooking(Booking& booking);
    std::string GenerateBookingReference(const std::string& hallId);
    static void ClearOldBookings();
};
