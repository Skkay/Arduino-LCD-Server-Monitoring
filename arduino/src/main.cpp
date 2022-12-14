#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SerialCommands.h>

char serial_command_buffer_[64];

SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\n", "//");
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void cmd_unrecognized(SerialCommands *sender, const char *cmd)
{
    sender->GetSerial()->println("ERROR: Unrecognized command [" + String(cmd) + "]");
}

/**
 * First parameter: X position
 * Second parameter: Y position
 * Third parameter: text to display
 *
 * E.g. cmd:lcd:write//0//1//second line text
 */
void cmd_lcd_write(SerialCommands *sender)
{
    char *x_pos_str = sender->Next();
    if (x_pos_str == NULL)
    {
        sender->GetSerial()->println("ERROR: X position is not provided");
        return;
    }
    int x_pos = atoi(x_pos_str);

    char *y_pos_str = sender->Next();
    if (y_pos_str == NULL)
    {
        sender->GetSerial()->println("ERROR: Y position is not provided");
        return;
    }
    int y_pos = atoi(y_pos_str);

    char *text = sender->Next();
    if (text == NULL)
    {
        sender->GetSerial()->println("ERROR: Text is not provided");
        return;
    }

    lcd.setCursor(x_pos, y_pos);
    lcd.print(text);

    sender->GetSerial()->println("INFO: Command [cmd:lcd:write] successfully executed with parameters [x_pos=" + String(x_pos_str) + "], [y_pos=" + String(y_pos_str) + "], [text=" + String(text) + "]");
}

void cmd_lcd_clear(SerialCommands *sender)
{
    lcd.clear();
    sender->GetSerial()->println("INFO: Command [cmd:lcd:clear] successfully executed");
}

void cmd_lcd_backlight(SerialCommands* sender)
{
    lcd.backlight();
    sender->GetSerial()->println("INFO: Command [cmd:lcd:backlight] successfully executed");
}

void cmd_lcd_noBacklight(SerialCommands* sender)
{
    lcd.noBacklight();
    sender->GetSerial()->println("INFO: Command [cmd:lcd:nobacklight] successfully executed");
}

SerialCommand cmd_lcd_write_("cmd:lcd:write", cmd_lcd_write);
SerialCommand cmd_lcd_clear_("cmd:lcd:clear", cmd_lcd_clear);
SerialCommand cmd_lcd_backlight_("cmd:lcd:backlight", cmd_lcd_backlight);
SerialCommand cmd_lcd_noBacklight_("cmd:lcd:nobacklight", cmd_lcd_noBacklight);

void setup()
{
    Serial.begin(115200);
    Serial.println("Initializing...");

    lcd.init();
    lcd.begin(16, 2);
    lcd.backlight();

    serial_commands_.SetDefaultHandler(&cmd_unrecognized);
    serial_commands_.AddCommand(&cmd_lcd_write_);
    serial_commands_.AddCommand(&cmd_lcd_clear_);
    serial_commands_.AddCommand(&cmd_lcd_backlight_);
    serial_commands_.AddCommand(&cmd_lcd_noBacklight_);

    Serial.println("Initialization done!");
}

void loop()
{
    serial_commands_.ReadSerial();
}
