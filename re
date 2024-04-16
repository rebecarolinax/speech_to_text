using Microsoft.AspNetCore.Mvc;
using System;
using System.Net.Http;
using System.Threading.Tasks;

namespace TranslationAPI.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class TranslationController : ControllerBase
    {
        private readonly HttpClient _httpClient;

        public TranslationController()
        {
            _httpClient = new HttpClient();
            _httpClient.DefaultRequestHeaders.Add("Ocp-Apim-Subscription-Key", "SUA_CHAVE_DE_ASSINATURA_AQUI");
        }

        [HttpPost("TextToSpeech")]
        public async Task<IActionResult> TextToSpeech([FromBody] string text)
        {
            try
            {
                var requestBody = $"{{\"text\":\"{text}\"}}";
                var content = new StringContent(requestBody);
                content.Headers.ContentType.MediaType = "application/json";

                var response = await _httpClient.PostAsync("https://westus.tts.speech.microsoft.com/cognitiveservices/v1", content);
                response.EnsureSuccessStatusCode();
                
                var audioStream = await response.Content.ReadAsStreamAsync();

                return File(audioStream, "audio/wav");
            }
            catch (Exception ex)
            {
                return BadRequest($"Error: {ex.Message}");
            }
        }

        [HttpPost("SpeechToText")]
        public async Task<IActionResult> SpeechToText([FromBody] byte[] audio)
        {
            try
            {
                var content = new ByteArrayContent(audio);
                content.Headers.TryAddWithoutValidation("Content-Type", "audio/wav");

                var response = await _httpClient.PostAsync("https://westus.stt.speech.microsoft.com/speech/recognition/conversation/cognitiveservices/v1", content);
                response.EnsureSuccessStatusCode();

                var text = await response.Content.ReadAsStringAsync();

                return Ok(text);
            }
            catch (Exception ex)
            {
                return BadRequest($"Error: {ex.Message}");
            }
        }
    }
}
