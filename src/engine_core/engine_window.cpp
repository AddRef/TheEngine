#include "engine_window.h"
#include "log.hxx"

#include "SDL.h"
#include "SDL_opengles2.h"
#include "SDL_egl.h"

namespace The
{
    
bool Window::Create(const Desc& desc)
{
    m_desc = desc;
    int result = SDL_Init(SDL_INIT_VIDEO);
    THE_ERROR_IF(result, "Failed to initialize SDL: " << SDL_GetError(), return false);
#ifdef WIN32
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif //WIN32
    m_window = SDL_CreateWindow(desc.title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, desc.width, desc.height, SDL_WINDOW_OPENGL);
    THE_ERROR_IF(!m_window, "Window creation failed: " << SDL_GetError(), return false);
    SetFullscreen(desc.fullscreen);
    return true;
}

void Window::Destroy()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Window::SetDimensions(uint32_t width, uint32_t height)
{
    m_desc.width = width;
    m_desc.height = height;
    SDL_SetWindowSize(m_window, m_desc.width, m_desc.height);
}

void Window::SetFullscreen(bool fullscreen)
{
    m_desc.fullscreen = fullscreen;
    SDL_SetWindowFullscreen(m_window, m_desc.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

void Window::SetTitle(const std::string& title)
{
    m_desc.title = title;
    SDL_SetWindowTitle(m_window, m_desc.title.c_str());
}

bool Window::Process() const
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
        {
            if (!event.key.repeat)
            {
                for (auto& cb : Emitter::GetCallback<IInputCallback>()) cb->OnKeyDown(convertSdlCodeToExternal(event.key.keysym.sym));
            }
        } break;
        case SDL_KEYUP:
        {
            for (auto& cb : Emitter::GetCallback<IInputCallback>()) cb->OnKeyUp(convertSdlCodeToExternal(event.key.keysym.sym));
        } break;
        case SDL_MOUSEBUTTONDOWN:
        {
            for (auto& cb : Emitter::GetCallback<IInputCallback>()) cb->OnMouseDown(convertSdlCodeToExternal(event.button.button), event.button.x, event.button.y);
        } break;
        case SDL_MOUSEBUTTONUP:
        {
            for (auto& cb : Emitter::GetCallback<IInputCallback>()) cb->OnMouseUp(convertSdlCodeToExternal(event.button.button), event.button.x, event.button.y);
        } break;
        case SDL_MOUSEMOTION:
        {
            for (auto& cb : Emitter::GetCallback<IInputCallback>()) cb->OnMouseMove(event.motion.x, event.motion.y);
        } break;
        case SDL_FINGERDOWN:
        {
            for (auto& cb : Emitter::GetCallback<IInputCallback>()) cb->OnFingerDown(event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
        }break;
        case SDL_FINGERUP:
        {
            for (auto& cb : Emitter::GetCallback<IInputCallback>()) cb->OnFingerUp(event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
        } break;
        case SDL_FINGERMOTION:
        {
            for (auto& cb : Emitter::GetCallback<IInputCallback>()) cb->OnFingerMove(event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
        } break;
        case SDL_MULTIGESTURE:
        {
            for (auto& cb : Emitter::GetCallback<IInputCallback>()) cb->OnMultiGesture(event.mgesture.numFingers, event.mgesture.x, event.mgesture.y, event.mgesture.dTheta, event.mgesture.dDist);
        } break;
        case SDL_QUIT:
            return false;
            break;
        }
    }
    return true;
}

InputKey Window::convertSdlCodeToExternal(uint32_t sdl_key)
{
    switch (sdl_key)
    {
    case SDLK_UNKNOWN:              return InputKey::Unknown; break;
    case SDLK_RETURN:               return InputKey::Return; break;
    case SDLK_ESCAPE:               return InputKey::Escape; break;
    case SDLK_BACKSPACE:            return InputKey::Backspace; break;
    case SDLK_TAB:                  return InputKey::Tab; break;
    case SDLK_SPACE:                return InputKey::Space; break;
    case SDLK_EXCLAIM:              return InputKey::Exclaim; break;
    case SDLK_QUOTEDBL:             return InputKey::Quotedbl; break;
    case SDLK_HASH:                 return InputKey::Hash; break;
    case SDLK_PERCENT:              return InputKey::Percent; break;
    case SDLK_DOLLAR:               return InputKey::Dollar; break;
    case SDLK_AMPERSAND:            return InputKey::Ampersand; break;
    case SDLK_QUOTE:                return InputKey::Quote; break;
    case SDLK_LEFTPAREN:            return InputKey::LeftParen; break;
    case SDLK_RIGHTPAREN:           return InputKey::RightParen; break;
    case SDLK_ASTERISK:             return InputKey::Asterisk; break;
    case SDLK_PLUS:                 return InputKey::Plus; break;
    case SDLK_COMMA:                return InputKey::Comma; break;
    case SDLK_MINUS:                return InputKey::Minus; break;
    case SDLK_PERIOD:               return InputKey::Period; break;
    case SDLK_SLASH:                return InputKey::Slash; break;
    case SDLK_0:                    return InputKey::Number0; break;
    case SDLK_1:                    return InputKey::Number1; break;
    case SDLK_2:                    return InputKey::Number2; break;
    case SDLK_3:                    return InputKey::Number3; break;
    case SDLK_4:                    return InputKey::Number4; break;
    case SDLK_5:                    return InputKey::Number5; break;
    case SDLK_6:                    return InputKey::Number6; break;
    case SDLK_7:                    return InputKey::Number7; break;
    case SDLK_8:                    return InputKey::Number8; break;
    case SDLK_9:                    return InputKey::Number9; break;
    case SDLK_COLON:                return InputKey::Colon; break;
    case SDLK_SEMICOLON:            return InputKey::Semicolon; break;
    case SDLK_LESS:                 return InputKey::Less; break;
    case SDLK_EQUALS:               return InputKey::Equals; break;
    case SDLK_GREATER:              return InputKey::Greater; break;
    case SDLK_QUESTION:             return InputKey::Question; break;
    case SDLK_AT:                   return InputKey::At; break;
    case SDLK_LEFTBRACKET:          return InputKey::LeftBracket; break;
    case SDLK_BACKSLASH:            return InputKey::Backslash; break;
    case SDLK_RIGHTBRACKET:         return InputKey::RightBracket; break;
    case SDLK_CARET:                return InputKey::Caret; break;
    case SDLK_UNDERSCORE:           return InputKey::Underscore; break;
    case SDLK_BACKQUOTE:            return InputKey::Backquote; break;
    case SDLK_a:                    return InputKey::A; break;
    case SDLK_b:                    return InputKey::B; break;
    case SDLK_c:                    return InputKey::C; break;
    case SDLK_d:                    return InputKey::D; break;
    case SDLK_e:                    return InputKey::E; break;
    case SDLK_f:                    return InputKey::F; break;
    case SDLK_g:                    return InputKey::G; break;
    case SDLK_h:                    return InputKey::H; break;
    case SDLK_i:                    return InputKey::I; break;
    case SDLK_j:                    return InputKey::J; break;
    case SDLK_k:                    return InputKey::K; break;
    case SDLK_l:                    return InputKey::L; break;
    case SDLK_m:                    return InputKey::M; break;
    case SDLK_n:                    return InputKey::N; break;
    case SDLK_o:                    return InputKey::O; break;
    case SDLK_p:                    return InputKey::P; break;
    case SDLK_q:                    return InputKey::Q; break;
    case SDLK_r:                    return InputKey::R; break;
    case SDLK_s:                    return InputKey::S; break;
    case SDLK_t:                    return InputKey::T; break;
    case SDLK_u:                    return InputKey::U; break;
    case SDLK_v:                    return InputKey::V; break;
    case SDLK_w:                    return InputKey::W; break;
    case SDLK_x:                    return InputKey::X; break;
    case SDLK_y:                    return InputKey::Y; break;
    case SDLK_z:                    return InputKey::Z; break;
    case SDLK_CAPSLOCK:             return InputKey::CapsLock; break;
    case SDLK_F1:                   return InputKey::F1; break;
    case SDLK_F2:                   return InputKey::F2; break;
    case SDLK_F3:                   return InputKey::F3; break;
    case SDLK_F4:                   return InputKey::F4; break;
    case SDLK_F5:                   return InputKey::F5; break;
    case SDLK_F6:                   return InputKey::F6; break;
    case SDLK_F7:                   return InputKey::F7; break;
    case SDLK_F8:                   return InputKey::F8; break;
    case SDLK_F9:                   return InputKey::F9; break;
    case SDLK_F10:                  return InputKey::F10; break;
    case SDLK_F11:                  return InputKey::F11; break;
    case SDLK_F12:                  return InputKey::F12; break;
    case SDLK_PRINTSCREEN:          return InputKey::PrintScreen; break;
    case SDLK_SCROLLLOCK:           return InputKey::ScrollLock; break;
    case SDLK_PAUSE:                return InputKey::Pause; break;
    case SDLK_INSERT:               return InputKey::Insert; break;
    case SDLK_HOME:                 return InputKey::Home; break;
    case SDLK_PAGEUP:               return InputKey::PageUp; break;
    case SDLK_DELETE:               return InputKey::Delete; break;
    case SDLK_END:                  return InputKey::End; break;
    case SDLK_PAGEDOWN:             return InputKey::PageDown; break;
    case SDLK_RIGHT:                return InputKey::Right; break;
    case SDLK_LEFT:                 return InputKey::Left; break;
    case SDLK_DOWN:                 return InputKey::Down; break;
    case SDLK_UP:                   return InputKey::Up; break;
    case SDLK_NUMLOCKCLEAR:         return InputKey::Numlockclear; break;
    case SDLK_KP_DIVIDE:            return InputKey::KP_Divide; break;
    case SDLK_KP_MULTIPLY:          return InputKey::KP_Multiply; break;
    case SDLK_KP_MINUS:             return InputKey::KP_Minus; break;
    case SDLK_KP_PLUS:              return InputKey::KP_Plus; break;
    case SDLK_KP_ENTER:             return InputKey::KP_Enter; break;
    case SDLK_KP_1:                 return InputKey::KP_1; break;
    case SDLK_KP_2:                 return InputKey::KP_2; break;
    case SDLK_KP_3:                 return InputKey::KP_3; break;
    case SDLK_KP_4:                 return InputKey::KP_4; break;
    case SDLK_KP_5:                 return InputKey::KP_5; break;
    case SDLK_KP_6:                 return InputKey::KP_6; break;
    case SDLK_KP_7:                 return InputKey::KP_7; break;
    case SDLK_KP_8:                 return InputKey::KP_8; break;
    case SDLK_KP_9:                 return InputKey::KP_9; break;
    case SDLK_KP_0:                 return InputKey::KP_0; break;
    case SDLK_KP_PERIOD:            return InputKey::KP_Period; break;
    case SDLK_APPLICATION:          return InputKey::Application; break;
    case SDLK_POWER:                return InputKey::Power; break;
    case SDLK_KP_EQUALS:            return InputKey::KP_Equals; break;
    case SDLK_F13:                  return InputKey::F13; break;
    case SDLK_F14:                  return InputKey::F14; break;
    case SDLK_F15:                  return InputKey::F15; break;
    case SDLK_F16:                  return InputKey::F16; break;
    case SDLK_F17:                  return InputKey::F17; break;
    case SDLK_F18:                  return InputKey::F18; break;
    case SDLK_F19:                  return InputKey::F19; break;
    case SDLK_F20:                  return InputKey::F20; break;
    case SDLK_F21:                  return InputKey::F21; break;
    case SDLK_F22:                  return InputKey::F22; break;
    case SDLK_F23:                  return InputKey::F23; break;
    case SDLK_F24:                  return InputKey::F24; break;
    case SDLK_EXECUTE:              return InputKey::Execute; break;
    case SDLK_HELP:                 return InputKey::Help; break;
    case SDLK_MENU:                 return InputKey::Menu; break;
    case SDLK_SELECT:               return InputKey::Select; break;
    case SDLK_STOP:                 return InputKey::Stop; break;
    case SDLK_AGAIN:                return InputKey::Again; break;
    case SDLK_UNDO:                 return InputKey::Undo; break;
    case SDLK_CUT:                  return InputKey::Cut; break;
    case SDLK_COPY:                 return InputKey::Copy; break;
    case SDLK_PASTE:                return InputKey::Paste; break;
    case SDLK_FIND:                 return InputKey::Find; break;
    case SDLK_MUTE:                 return InputKey::Mute; break;
    case SDLK_VOLUMEUP:             return InputKey::VolumeUp; break;
    case SDLK_VOLUMEDOWN:           return InputKey::VolumeDown; break;
    case SDLK_KP_COMMA:             return InputKey::KP_Comma; break;
    case SDLK_KP_EQUALSAS400:       return InputKey::KP_Equalsas400; break;
    case SDLK_ALTERASE:             return InputKey::Alterase; break;
    case SDLK_SYSREQ:               return InputKey::Sysreq; break;
    case SDLK_CANCEL:               return InputKey::Cancel; break;
    case SDLK_CLEAR:                return InputKey::Clear; break;
    case SDLK_PRIOR:                return InputKey::Prior; break;
    case SDLK_RETURN2:              return InputKey::Return2; break;
    case SDLK_SEPARATOR:            return InputKey::Separator; break;
    case SDLK_OUT:                  return InputKey::Out; break;
    case SDLK_OPER:                 return InputKey::Oper; break;
    case SDLK_CLEARAGAIN:           return InputKey::Clearagain; break;
    case SDLK_CRSEL:                return InputKey::Crsel; break;
    case SDLK_EXSEL:                return InputKey::Exsel; break;
    case SDLK_KP_00:                return InputKey::KP_00; break;
    case SDLK_KP_000:               return InputKey::KP_000; break;
    case SDLK_THOUSANDSSEPARATOR:   return InputKey::Thousandsseparator; break;
    case SDLK_DECIMALSEPARATOR:     return InputKey::Decimalseparator; break;
    case SDLK_CURRENCYUNIT:         return InputKey::Currencyunit; break;
    case SDLK_CURRENCYSUBUNIT:      return InputKey::Currencysubunit; break;
    case SDLK_KP_LEFTPAREN:         return InputKey::KP_Leftparen; break;
    case SDLK_KP_RIGHTPAREN:        return InputKey::KP_Rightparen; break;
    case SDLK_KP_LEFTBRACE:         return InputKey::KP_Leftbrace; break;
    case SDLK_KP_RIGHTBRACE:        return InputKey::KP_Rightbrace; break;
    case SDLK_KP_TAB:               return InputKey::KP_Tab; break;
    case SDLK_KP_BACKSPACE:         return InputKey::KP_Backspace; break;
    case SDLK_KP_A:                 return InputKey::KP_A; break;
    case SDLK_KP_B:                 return InputKey::KP_B; break;
    case SDLK_KP_C:                 return InputKey::KP_C; break;
    case SDLK_KP_D:                 return InputKey::KP_D; break;
    case SDLK_KP_E:                 return InputKey::KP_E; break;
    case SDLK_KP_F:                 return InputKey::KP_F; break;
    case SDLK_KP_XOR:               return InputKey::KP_Xor; break;
    case SDLK_KP_POWER:             return InputKey::KP_Power; break;
    case SDLK_KP_PERCENT:           return InputKey::KP_Percent; break;
    case SDLK_KP_LESS:              return InputKey::KP_Less; break;
    case SDLK_KP_GREATER:           return InputKey::KP_Greater; break;
    case SDLK_KP_AMPERSAND:         return InputKey::KP_Ampersand; break;
    case SDLK_KP_DBLAMPERSAND:      return InputKey::KP_DblAmpersand; break;
    case SDLK_KP_VERTICALBAR:       return InputKey::KP_Verticalbar; break;
    case SDLK_KP_DBLVERTICALBAR:    return InputKey::KP_DblVerticalbar; break;
    case SDLK_KP_COLON:             return InputKey::KP_Colon; break;
    case SDLK_KP_HASH:              return InputKey::KP_Hash; break;
    case SDLK_KP_SPACE:             return InputKey::KP_Space; break;
    case SDLK_KP_AT:                return InputKey::KP_At; break;
    case SDLK_KP_EXCLAM:            return InputKey::KP_Exclam; break;
    case SDLK_KP_MEMSTORE:          return InputKey::KP_MemStore; break;
    case SDLK_KP_MEMRECALL:         return InputKey::KP_MemRecall; break;
    case SDLK_KP_MEMCLEAR:          return InputKey::KP_MemClear; break;
    case SDLK_KP_MEMADD:            return InputKey::KP_MemAdd; break;
    case SDLK_KP_MEMSUBTRACT:       return InputKey::KP_MemSubtract; break;
    case SDLK_KP_MEMMULTIPLY:       return InputKey::KP_MemMultiply; break;
    case SDLK_KP_MEMDIVIDE:         return InputKey::KP_Memdivide; break;
    case SDLK_KP_PLUSMINUS:         return InputKey::KP_Plus; break;
    case SDLK_KP_CLEAR:             return InputKey::KP_Clear; break;
    case SDLK_KP_CLEARENTRY:        return InputKey::KP_Clearentry; break;
    case SDLK_KP_BINARY:            return InputKey::KP_Binary; break;
    case SDLK_KP_OCTAL:             return InputKey::KP_Octal; break;
    case SDLK_KP_DECIMAL:           return InputKey::KP_Decimal; break;
    case SDLK_KP_HEXADECIMAL:       return InputKey::KP_Hexadecimal; break;
    case SDLK_LCTRL:                return InputKey::LCtrl; break;
    case SDLK_LSHIFT:               return InputKey::LShift; break;
    case SDLK_LALT:                 return InputKey::LAlt; break;
    case SDLK_LGUI:                 return InputKey::LGui; break;
    case SDLK_RCTRL:                return InputKey::RCtrl; break;
    case SDLK_RSHIFT:               return InputKey::RShift; break;
    case SDLK_RALT:                 return InputKey::RAlt; break;
    case SDLK_RGUI:                 return InputKey::RGui; break;
    case SDLK_MODE:                 return InputKey::Mode; break;
    case SDLK_AUDIONEXT:            return InputKey::Audionext; break;
    case SDLK_AUDIOPREV:            return InputKey::Audioprev; break;
    case SDLK_AUDIOSTOP:            return InputKey::Audiostop; break;
    case SDLK_AUDIOPLAY:            return InputKey::Audioplay; break;
    case SDLK_AUDIOMUTE:            return InputKey::Audiomute; break;
    case SDLK_MEDIASELECT:          return InputKey::Mediaselect; break;
    case SDLK_WWW:                  return InputKey::Www; break;
    case SDLK_MAIL:                 return InputKey::Mail; break;
    case SDLK_CALCULATOR:           return InputKey::Calculator; break;
    case SDLK_COMPUTER:             return InputKey::Computer; break;
    case SDLK_AC_SEARCH:            return InputKey::AC_Search; break;
    case SDLK_AC_HOME:              return InputKey::AC_Home; break;
    case SDLK_AC_BACK:              return InputKey::AC_Back; break;
    case SDLK_AC_FORWARD:           return InputKey::AC_Forward; break;
    case SDLK_AC_STOP:              return InputKey::AC_Stop; break;
    case SDLK_AC_REFRESH:           return InputKey::AC_Refresh; break;
    case SDLK_AC_BOOKMARKS:         return InputKey::AC_Bookmarks; break;
    case SDLK_BRIGHTNESSDOWN:       return InputKey::BrightnessDown; break;
    case SDLK_BRIGHTNESSUP:         return InputKey::BrightnessUp; break;
    case SDLK_DISPLAYSWITCH:        return InputKey::DisplaySwitch; break;
    case SDLK_KBDILLUMTOGGLE:       return InputKey::KbdillumToggle; break;
    case SDLK_KBDILLUMDOWN:         return InputKey::KbdillumDown; break;
    case SDLK_KBDILLUMUP:           return InputKey::KbdillumUp; break;
    case SDLK_EJECT:                return InputKey::Eject; break;
    case SDLK_SLEEP:                return InputKey::Sleep; break;

    case SDL_BUTTON_LEFT:           return InputKey::MouseButtonLeft; break;
    case SDL_BUTTON_MIDDLE:         return InputKey::MouseButtonMiddle; break;
    case SDL_BUTTON_RIGHT:          return InputKey::MouseButtonRight; break;
    default:
        return InputKey::Unknown;
    }
}

}